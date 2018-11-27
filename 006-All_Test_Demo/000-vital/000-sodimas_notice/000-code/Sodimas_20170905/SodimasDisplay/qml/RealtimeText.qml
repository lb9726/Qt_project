import QtQuick 1.1

UiElement {
    property string msgL1: ""
    property string msgL2: ""
    property color textColor: "white"
    property bool isAlignCenter: false
    Text {
        width: parent.width
        height: parent.height/2-2
        font.pixelSize: width/18
        text: msgL1
        color: textColor
        horizontalAlignment: isAlignCenter ? Text.AlignHCenter:Text.AlignLeft
    }

    Text {
        y: parent.height/2
        width: parent.width
        height: parent.height/2-2
        font.pixelSize: width/18
        text: msgL2
        color: textColor
        horizontalAlignment: isAlignCenter ? Text.AlignHCenter : Text.AlignLeft
    }

    function updateText(msg1, msg2)
    {
        msgL1 = msg1;
        msgL2 = msg2;
    }

    function appendText(m1, m2)
    {
        msgL1 += m1;
        msgL2 += m2;
    }
}
