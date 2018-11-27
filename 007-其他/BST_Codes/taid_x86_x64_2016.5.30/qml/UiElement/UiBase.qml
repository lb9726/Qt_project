import QtQuick 2.0
Rectangle{
    id: root
    property alias selecteFlag: scope.activeFocus
    property int mininalSize: 20
    Drag.active: dragArea.drag.active
    Drag.hotSpot.x: root.width/2
    Drag.hotSpot.y: root.height/2

    signal sig_xResize(int deltX);
    signal sig_yResize(int deltY);

    signal sig_moving(int mo_x,int mo_y);
    signal sig_resizing(int re_width,int re_height);

    FocusScope {
        id: scope
        width: childrenRect.width; height: childrenRect.height

        onActiveFocusChanged: {
            console.debug("onActiveFocusChanged"+activeFocus)
        }

        Item{
            width: root.width
            height: root.height
            visible: root.selecteFlag

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
                            //                            if((mouse.x - tempMousex)>0){
                            //                                root.width  += mouse.x - tempMousex
                            //                            }else{
                            //                                if((root.width  + mouse.x - tempMousex)>20){
                            //                                    root.width  += mouse.x - tempMousex
                            //                                }
                            //                            }
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
                            //                            if((mouse.y - tempMousey)>0){
                            //                                root.height  += mouse.y - tempMousey
                            //                            }else{
                            //                                if((root.height  + mouse.y - tempMousey)>20){
                            //                                    root.height  += mouse.y - tempMousey
                            //                                }
                            //                            }
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

                            //                            if((mouse.y - tempMousey)>0){
                            //                                root.height  += mouse.y - tempMousey
                            //                            }else{
                            //                                if((root.height  + mouse.y - tempMousey)>20){
                            //                                    root.height  += mouse.y - tempMousey
                            //                                }
                            //                            }

                            //                            if((mouse.x - tempMousex)>0){
                            //                                root.width  += mouse.x - tempMousex
                            //                            }else{
                            //                                if((root.width  + mouse.x - tempMousex)>20){
                            //                                    root.width  += mouse.x - tempMousex
                            //                                }
                            //                            }
                        }
                    }

                    onEntered: {
                        cursorShape = Qt.SizeFDiagCursor
                    }
                }
            }
        }
    }
    color: selecteFlag ? "red":"green";

    MouseArea{
        id:dragArea
        anchors.fill: parent
        drag.target: parent
        onPressed: {
            scope.focus = true
        }
        onClicked: {

        }
        z:-1
    }

    property bool xrangeCtrlFlag: false //当控件宽度超出父控件时为:true
    property bool yrangeCtrlFlag: false //当控件宽度超出父控件时为:true

    onXChanged: {
        if(width < parent.width)
            xrangeCtrlFlag = false;
        sig_moving(x,y);
    }

    onYChanged: {
        //console.log("y change :"+y)
        if(height < parent.height)
            yrangeCtrlFlag = false;
        sig_moving(x,y);
    }

    onWidthChanged: {
        //console.log("width change :"+width)
        if(x + width >= parent.width){
            xrangeCtrlFlag  = true;
        }
        sig_resizing(width,height);
    }

    onHeightChanged: {
        //console.log("height change :"+height)
        if(y + height >= parent.width){
            yrangeCtrlFlag  = true;
        }
        sig_resizing(width,height);
    }

    onSig_xResize: {
        if(xrangeCtrlFlag)
            return
        if(deltX > 0){
            root.width  += deltX
        }else{
            if((root.width  + deltX) > mininalSize){
                root.width  += deltX
            }
        }
    }

    onSig_yResize: {
        if(yrangeCtrlFlag)
            return
        if((deltY) > 0){
            root.height  += deltY
        }else{
            if((root.height  + deltY) > mininalSize){
                root.height  += deltY
            }
        }
    }

    Component.onCompleted: {
        if(x > parent.width){
            x = 0
        }
        if(y > parent.height){
            y = 0
        }
    }
}
