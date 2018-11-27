import QtQuick 2.3
import QtQuick.Window 2.2

Window {
    visible: true
    width: 360
    height: 360
//    MouseArea {
//        anchors.fill: parent
//        onClicked: {
//            Qt.quit();
//        }
//    }

    Item {
        x:10; y:10
        width: parent.width - 10
        height: parent.height -10
//        anchors.centerIn: parent
    }
}

