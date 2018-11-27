import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Styles 1.3

Rectangle {
    id: rect_btn
    property string rectColor: "grey"
    property real opa: 1
    property string src: "qrc:///image/grey_Close.png"
    signal buttonclick()
    width: 24
    height: 24
    color: rectColor
    opacity: opa
    Image {
        id: close_pic
        anchors.fill: parent
        source: src
    }
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onHoveredChanged: {
            src = "qrc:///image/red_Close.png"
        }
        onExited: {
            src = "qrc:///image/grey_Close.png"
        }
        onPressed: {
            opa = 0.6
        }
        onReleased: {
            opa = 1
        }
        onClicked: {
            buttonclick();
        }
    }
}
