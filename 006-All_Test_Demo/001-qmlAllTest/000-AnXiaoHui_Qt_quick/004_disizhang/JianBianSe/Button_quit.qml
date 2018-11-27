import QtQuick 2.0
import QtQuick.Controls 1.2
Rectangle {
    width: 320
    height: 240
    color: "gray"
    Button {
        text: "Quit"
        anchors.centerIn: parent;
        iconName: "qrc:/images/buttonIcon1.png"
        onClicked: {
            Qt.quit()
        }
    }
}
