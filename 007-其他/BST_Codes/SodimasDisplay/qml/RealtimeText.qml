import QtQuick 1.1

UiElement {
    property string msgL1: ""
    property string msgL2: ""

    Text{
        width: parent.width
        height: parent.height/2
        font.pixelSize: width/24
        text:msgL1
    }

    Text{
        y:parent.height/2
        width: parent.width
        height: parent.height/2
        font.pixelSize: width/24
        text:msgL2
    }

    function updateText(msg1,msg2){
        msgL1 = msg1;
        msgL2 = msg2;
    }
}
