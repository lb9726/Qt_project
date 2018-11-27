import QtQuick 2.0

Rectangle {
    signal buttonClicked()
    width: 100
    height: 50
    color: "red"
    border.color: "black"
    border.width: 1
    Text {
        id: name
        text: qsTr("text")
        anchors.centerIn: parent
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            buttonClicked();
        }
    }
}

