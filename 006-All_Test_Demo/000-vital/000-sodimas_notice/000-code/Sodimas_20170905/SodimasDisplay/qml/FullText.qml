import QtQuick 1.1

UiElement {
    id: id_fulltext
    property string  msgLine1:""
    property string  msgLine2:""
    property string  msgLine3:""
    property string  msgLine4:""
    //property string  MsgLine4:""
    property real textSize: width/18
    property color textColor: "white"
    clip: true

    Column {
        anchors.fill: parent
        Text {
            width: parent.width
            height: parent.height/4
            font.pixelSize: textSize
            color: textColor
            verticalAlignment: Text.AlignVCenter
            text: msgLine1
        }
        Text{
            width: parent.width
            height: parent.height/4
            font.pixelSize: textSize
            color: textColor
            verticalAlignment: Text.AlignVCenter
            text: msgLine2
        }
        Text {
            width: parent.width
            height: parent.height/4
            font.pixelSize: textSize
            color: textColor
            verticalAlignment: Text.AlignVCenter
            text: msgLine3
        }
        Text {
            width: parent.width
            height: parent.height/4
            font.pixelSize: textSize
            color: textColor
            verticalAlignment: Text.AlignVCenter
            text: msgLine4
        }
    }

    function updateText(msg1, msg2, msg3, msg4)
    {
        msgLine1 = msg1;
        msgLine2 = msg2;
        msgLine3 = msg3;
        msgLine4 = msg4;
    }
}
