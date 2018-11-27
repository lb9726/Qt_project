import QtQuick 2.4

Rectangle {
    width: 100
    height: 30

    Text {
        id:idtext
        //width: parent.width
        height:parent.height
        text:"123456789abcdefghijklmnopqrstuvwxyz"
        color:"red"
        verticalAlignment: Text.AlignVCenter
        SequentialAnimation on x {
        loops: Animation.Infinite
            PropertyAnimation {
                from:0
                to: -idtext.width
                duration: 15000
            }
       }
    }
}

