import QtQuick 2.3
import "../editor.js" as Editor

Element{
    opacity: 0.2
    visible: !previewModel

    Image{
        width: parent.width/2
        height: parent.height/2
        anchors.centerIn: parent
        source:"qrc:/images/media.png"
        z:1
    }

    Rectangle{
        anchors.fill: parent
        anchors.margins: 2
        border.width: 2
        border.color: "white"
        color: "transparent"
    }
}
