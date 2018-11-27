import QtQuick 2.7
import QtQuick.Window 2.2

Window {
    visible: true
    width: 1027
    height: 768
    title: qsTr("Hello World")

    Image {
        source: "qrc:/images/panel_Background/video_lastFrame.png"
    }

    BackGroundVideo {
        width: parent.width
        height: parent.height
    }

    Instrument_Panel {
        x: 0
        y: 370
    }

    Text {
        id: floor_Text_ID
        text: "B1"
        x: parent.width / 2.0
        y: parent.height / 2.0 - 150
        font.bold: true
        font.pixelSize: 30
        color: "white"
    }

    Connections {
        target: DateAndTime
        onSigAutoTestChanged: {
            floor_Text_ID.text = pFloor
        }
    }
}
