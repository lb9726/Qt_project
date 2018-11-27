import QtQuick 2.0

Rectangle {
    property int num: 0
    id: rectTest
    color: "yellow"
    MouseArea {
        anchors.fill: parent
        onClicked: {
            mtest.changeColor();
        }
    }
}
