import QtQuick 2.4
import QtQuick.Window 2.2

Window {
    visible: true
    width: 600
    height: 600
//    MainForm {
//        anchors.fill: parent
//        mouseArea.onClicked: {
//            Qt.quit();
//        }
//    }
    Test {
        anchors.fill: parent
        mouseArea.onClicked: {
            Qt.quit();
        }
    }
}
