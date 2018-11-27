import QtQuick 2.5
import QtQuick.Window 2.0

Rectangle {
    width: 640
    height: 480
    Rectangle {
        objectName: "rect1"
        id: rect
        color: "red"
        width: parent.width
        height: parent.height
    }
}
