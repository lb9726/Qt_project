import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtMultimedia 5.7
import QtQuick.Controls.Material 2.0

ApplicationWindow {
    id: id_mediaScreenHelper
    visible: true
    property int windowsHeight: 740
    property int windowsWidth: 1440

    maximumHeight:windowsHeight
    maximumWidth:windowsWidth

    minimumHeight:windowsHeight
    minimumWidth:windowsWidth
    Material.theme: Material.Light
    Material.accent: Material.Green
    title: qsTr("MediaScreen Content management") + qsTr("  V2.2.0")

    Rectangle {
        width: parent.width
        height: 30
        color: "transparent"
        Label {
            id: title_label
            x: 15
            text: qsTr("参数设置")
            color: "steelblue"
            font.bold: true
            font.pixelSize: 20
        }
    }

    Rectangle {
        y: 30
        color: "steelblue"
        width: parent.width
        height: 3
    }

    GridAreaLayout {
        id: wholeArea
        x: 8
        y: 40
    }
}
