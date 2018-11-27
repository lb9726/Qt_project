import QtQuick 2.0

Rectangle {
    width: 400
    height: 400
    Text {
        width: 150
        height: 150
        wrapMode: Text.WordWrap
        font.bold: true
        font.pixelSize: 24
        font.underline: true
        text: "Hello Blue <font color=\"blue\">Text</font>"
        anchors.centerIn: parent
    }
}
