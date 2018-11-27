import QtQuick 2.7
import QtQuick.Window 2.2

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Rectangle {
        id: player

        width: 20 * Screen.logicalPixelDensity
        height: width

        radius: width / 2
        color: "#006325"

        property double speed: 2 * Screen.logicalPixelDensity
    }
}
