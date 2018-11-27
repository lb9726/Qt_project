import QtQuick 2.4
import QtQuick.Window 2.2

Window {
    visible: true
    width: 460
    height: 460
    MainForm {
        anchors.fill: parent
        mouseArea.onClicked: {
            Qt.quit();
        }

    }
}
