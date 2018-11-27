import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 1.3

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    TabView {
        anchors.fill: parent

        Tab {
            title: "Color"
            ColorDialogTab {}
        }

        Tab {
            title: "Message"
            MessageDialogTab {}
        }
    }
}
