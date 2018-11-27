import QtQuick 2.0
import "../editor.js" as Editor
Rectangle{
    id: elementItem
    property rect rectH:Qt.rect(50,50,50,50)
    property rect rectV:Qt.rect(50,50,50,50)
    property string elementType: "untitle"
    property bool oriention: false
    property bool resizeCtrl: true
    property bool previewModel: gAutoMode
    property var panelAttr: null
    property string coverImag: panelAttr.m_background

    property bool selecteFlag: false
    property alias elementFocus: scope.focus

    property int panelIndex:0
    property int mininalSize: 20
    property bool initFinish:false

    signal sig_moving(int mo_x,int mo_y);
    signal sig_resizing(int re_width,int re_height);

    //enabled: !gAutoMode

    border.color: "red"
    border.width: (elementItem.selecteFlag || elementItem.elementFocus) &&!previewModel ? 1 : 0
    color: "transparent" //coverImag == "" ? "white":"transparent"
    z:selecteFlag || elementFocus ? 10 : 0

    Drag.active: dragArea.drag.active
    Drag.hotSpot.x: elementItem.width/2
    Drag.hotSpot.y: elementItem.height/2
    onOrientionChanged: {
        initFinish = false
        if(oriention){
            rectH.x = x;
            rectH.y = y;
            x = rectV.x
            y = rectV.y
        }else{
            rectV.x = x;
            rectV.y = y;
            x = rectH.x
            y = rectH.y
        }
        initFinish = true
    }

    Item{
        id:id_ctrlPoint
        width: elementItem.width
        height: elementItem.height
        visible: (elementItem.selecteFlag || elementItem.elementFocus)&& !gAutoMode
        enabled: resizeCtrl
        property int ctrlPointW:Math.max(width,height)/20
        property int ctrlPointH:Math.max(width,height)/20

        //property int ctrlPointW:width > 900 ? 80 :width/10
        //property int ctrlPointH:height > 900 ? 80 :height/10

        Rectangle{
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: -id_ctrlPoint.ctrlPointW/2
            width: id_ctrlPoint.ctrlPointW
            height: id_ctrlPoint.ctrlPointH
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
                        ctrl_xResize(mouse.x - tempMousex);
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
            anchors.bottomMargin: -id_ctrlPoint.ctrlPointH/2
            width: id_ctrlPoint.ctrlPointW
            height: id_ctrlPoint.ctrlPointH
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
                        ctrl_yResize(mouse.y - tempMousey);
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
            anchors.rightMargin: -id_ctrlPoint.ctrlPointW/2
            anchors.bottomMargin: -id_ctrlPoint.ctrlPointH/2
            width: id_ctrlPoint.ctrlPointW
            height: id_ctrlPoint.ctrlPointH
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
                        ctrl_yResize(mouse.y - tempMousey);
                        ctrl_xResize(mouse.x - tempMousex);
                    }
                }

                onEntered: {
                    cursorShape = Qt.SizeFDiagCursor
                }
            }
        }
    }

    MouseArea {
        id:dragArea
        enabled: !previewModel
        anchors.fill: parent
        drag.target: parent

        onPressed: {
            scope.forceActiveFocus();
            elementItem.selecteFlag = true
            Editor.selectAction(elementType,panelIndex);
        }

        onPressAndHold: {
            //deleteElement();
        }

        z:-10
    }

    FocusScope {
        id: scope
        width: childrenRect.width;
        height: childrenRect.height
    }

    Keys.onDeletePressed: deleteElement();
    Keys.onUpPressed: {
        y= y-1
    }
    Keys.onDownPressed: {
        y= y+1
    }
    Keys.onLeftPressed: {
        x= x-1
    }
    Keys.onRightPressed: {
        x = x+1
    }

    function deleteElement(){
        GUiEditor.removeUIElement(elementType);
        Editor.destroyCreatedPanel(elementType)
        elementItem.destroy();
        panelAttr.destroy();
    }

    onSelecteFlagChanged: {
        if(selecteFlag){
            scope.forceActiveFocus();
            panelAttr.visible = true
        }else{
            panelAttr.visible = false
        }
    }

    onXChanged: {
        if(x < 0){
            x = 0;
        }

        if(x + width > parent.width){
            x = parent.width - width;
        }
        sig_moving(x,y);
        geoUpdate();
    }

    onYChanged: {
        if(y < 0)
            y = 0
        if(y+height > parent.height){
            y= parent.height - height
        }
        sig_moving(x,y);
        geoUpdate();
    }

    function ctrl_xResize(deltX){
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
    function ctrl_yResize(deltY){
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

    //Attr Panel input
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

    function attrPanelInit(){
        var componentattr = Qt.createComponent("../attr/"+elementType+"Attr.qml");
        if (componentattr.status === Component.Ready){
            panelAttr = componentattr.createObject(id_attrArea,{
                                                       "panel":elementItem,
                                                       "visible":true,
                                                       "elementName":elementType
                                                   });
            if(panelAttr != null)
            {
                elementItem.sig_moving.connect(panelAttr.setcord);
                elementItem.sig_resizing.connect(panelAttr.setsize);
                panelAttr.sig_attr_resize.connect(elementItem.slt_setSize);
                sig_moving(x,y);
                sig_resizing(elementItem.width,elementItem.height);

                scope.forceActiveFocus();
                elementItem.selecteFlag = true;
                initFinish = true
            }
        }
    }

    Component.onCompleted: {
        if(oriention){
            x = rectV.x;
            y = rectV.y;
            width = rectV.width;
            height = rectV.height;
        }else{
            x = rectH.x
            y = rectH.y
            width = rectH.width;
            height = rectH.height;
        }
        attrPanelInit();
    }

    Component.onDestruction: {
        if(panelAttr){
            Editor.destroyCreatedPanel(elementType)
            panelAttr.destroy();
        }
        if(elementItem)
            elementItem.destroy();
    }

    onWidthChanged:{geoUpdate();}
    onHeightChanged:{geoUpdate();}

    function geoUpdate(){
        if(initFinish){
            GUiEditor.resizeRect(elementType,Qt.rect(x,y,width,height));
        }
    }
}
