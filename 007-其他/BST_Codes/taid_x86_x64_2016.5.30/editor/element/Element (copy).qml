import QtQuick 2.0
import "../editor.js" as Editor
Rectangle{
    id: elementItem
    property size posH:Qt.size(0,0)
    property bool selecteFlag: false
    property alias elementFocus: scope.focus
    property string elementType: "untitle"
    property int panelIndex:0
    property int mininalSize: 20
    property bool initFinish:false
    property var panelAttr: null
    property string coverImag: panelAttr.m_background

    signal sig_xResize(int deltX);
    signal sig_yResize(int deltY);
    signal sig_moving(int mo_x,int mo_y);
    signal sig_resizing(int re_width,int re_height);

    Drag.active: dragArea.drag.active
    Drag.hotSpot.x: elementItem.width/2
    Drag.hotSpot.y: elementItem.height/2
    border.width: selecteFlag ? 1 : 0
    border.color: "red"
    color: coverImag == "" ? "white":"transparent"

    z:selecteFlag || elementFocus ? 10 : 0
    Keys.onDeletePressed: deleteElement();

    FocusScope {
        id: scope
        width: childrenRect.width; height: childrenRect.height
    }

    Item{
        width: elementItem.width
        height: elementItem.height
        visible: elementItem.selecteFlag || elementItem.elementFocus

        Rectangle{
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: -3
            width: 6
            height: 6
            color: "blue"
            MouseArea{
                anchors.fill: parent
                hoverEnabled:true
                property int tempMousex: 0;

                onPressed: {
                    tempMousex = mouse.x
                }

                onPositionChanged:{
                    if(pressed){
                        sig_xResize(mouse.x - tempMousex);
                    }

                }

                onEntered: {
                    cursorShape = Qt.SizeHorCursor
                }
            }
        }

        Rectangle{
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: -3
            width: 6
            height: 6
            color: "blue"
            MouseArea{
                anchors.fill: parent
                hoverEnabled:true
                property int tempMousey: 0;

                onPressed: {
                    tempMousey = mouse.y
                }
                onPositionChanged:{
                    if(pressed){
                        sig_yResize(mouse.y - tempMousey);
                    }
                }
                onEntered: {
                    cursorShape = Qt.SizeVerCursor
                }
            }
        }

        Rectangle{
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.rightMargin: -3
            anchors.bottomMargin: -3
            width: 6
            height: 6
            color: "blue"
            MouseArea{
                anchors.fill: parent
                hoverEnabled:true
                property int tempMousex: 0;
                property int tempMousey: 0;

                onPressed: {
                    tempMousex = mouse.x
                    tempMousey = mouse.y
                }

                onPositionChanged:{
                    if(pressed){
                        sig_yResize(mouse.y - tempMousey);
                        sig_xResize(mouse.x - tempMousex);
                    }
                }

                onEntered: {
                    cursorShape = Qt.SizeFDiagCursor
                }
            }
        }
    }

    MouseArea{
        id:dragArea
        anchors.fill: parent
        drag.target: parent
        onPressed: {
            scope.forceActiveFocus();
            elementItem.selecteFlag = true
            Editor.selectAction(elementType,panelIndex);
        }
        onClicked: {
            //scope.forceActiveFocus();
            //elementItem.selecteFlag = true
            //Editor.selectAction(elementType,panelIndex);
        }
        z:-10
    }

    onXChanged: {
        if(x < 0)
            x = 0
        if(x + width > parent.width){
            x = 0;
            //x = parent.width - width;
        }
        sig_moving(x,y);
        reSize();

    }

    onYChanged: {
        if(y < 0)
            y = 0
        if(y+height > parent.height){
            y=0;
            //y= parent.height - height
        }
        sig_moving(x,y);
        reSize();
    }

    onWidthChanged: {
        reSize();
    }

    onHeightChanged: {
        reSize();
    }

    onSig_xResize: {
        if(deltX > 0){
            if(elementItem.x + elementItem.width + deltX <= parent.width)
                elementItem.width  += deltX
        }else{
            if((elementItem.width  + deltX) > mininalSize){
                elementItem.width  += deltX
            }
        }
        sig_resizing(elementItem.width,elementItem.height)
    }

    onSig_yResize: {
        if(deltY > 0){
            if(elementItem.y + elementItem.height  + deltY <= parent.height)
                elementItem.height  += deltY
        }else{
            if((elementItem.height  + deltY) > mininalSize){
                elementItem.height  += deltY
            }
        }
        sig_resizing(elementItem.width,elementItem.height)
    }

    onSelecteFlagChanged: {
        if(selecteFlag){
            scope.forceActiveFocus();
            panelAttr.visible = true
        }else{
            panelAttr.visible = false
        }
    }

    Component.onCompleted: {
        var componentattr = Qt.createComponent("../attr/"+elementType+"Attr.qml");
        if (componentattr.status === Component.Ready){
            panelAttr = componentattr.createObject(attrArea);
            if(panelAttr != null){
                elementItem.sig_moving.connect(panelAttr.setcord);
                elementItem.sig_resizing.connect(panelAttr.setsize);
                panelAttr.sig_attr_resize.connect(elementItem.slt_setSize);
                panelAttr.elementName = elementType;
            }
        }

        sig_moving(x,y);
        sig_resizing(elementItem.width,elementItem.height);
        scope.forceActiveFocus();
        elementItem.selecteFlag = true;
        initFinish = true

        console.log(posH)
    }

    function deleteElement(){
        Editor.destroyCreatedPanel(elementType)
        elementItem.destroy();
        panelAttr.destroy();
    }

    function slt_setSize(sX,sY,sWidth,sHeight){
        if(!(sX < 0||sX >parent.width))
            x = sX;
        if(!(sX < 0||sX >parent.height))
            y = sY;
        if(x + sWidth <= parent.width)
            width = sWidth;
        if(y + sHeight <= parent.height)
            height = sHeight;
        sig_moving(x,y);
        sig_resizing(elementItem.width,elementItem.height);
    }

    function reSize(){
        if(initFinish)
            GUiEditor.resizeRect(elementType,Qt.rect(x,y,width,height));
    }
}
