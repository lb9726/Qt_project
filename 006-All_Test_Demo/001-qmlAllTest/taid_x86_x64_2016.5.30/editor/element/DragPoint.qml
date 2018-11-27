import QtQuick 2.5
Item{
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
