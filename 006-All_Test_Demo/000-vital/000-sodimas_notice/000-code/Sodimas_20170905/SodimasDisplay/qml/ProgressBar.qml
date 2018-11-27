import QtQuick 1.1

UiElement {
    property int value: 0
    property real percentLength: width/100

    Text {
        anchors.centerIn: parent
        text: value
        color: "black"
        z: 10
    }

    Rectangle {
        width: parent.width
        height: parent.height*0.8
        anchors.centerIn: parent
        radius: height/2
        border.color: "yellow"
        border.width: 2
        clip: true

        Rectangle {
            x: 2
            color: "green"
            height: parent.height - 2
            width: percentLength*value
            radius: height/2
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    function updateProgress(progress)
    {
        if(progress >= 0 && progress <= 100)
        {
            value = progress
        }
    }
}
