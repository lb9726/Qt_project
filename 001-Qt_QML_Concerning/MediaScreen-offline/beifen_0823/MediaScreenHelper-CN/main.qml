import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtMultimedia 5.7
import QtQuick.Controls.Material 2.0

ApplicationWindow {
    id:id_mediaScreenHelper
    visible: true
    property int windowsHeight: 700
    property int windowsWidth: 1200 - 40

    property bool resetEnabled: false  //用户是否使能了Reset功能
    //    property alias multiMediaView: id_multiMediaView
    //    property alias parameterView: id_parameterView

    maximumHeight:windowsHeight
    maximumWidth:windowsWidth

    minimumHeight:windowsHeight
    minimumWidth:windowsWidth
    Material.theme: Material.Light
    Material.accent: Material.Green
    title: qsTr("MediaScreenHelper") + qsTr("  V1.6.0")

//    background:Image{
//        anchors.fill: parent
//        opacity: 0.8
//        source: "qrc:///image/background.png"
//    }
    Rectangle {
        width: parent.width
        height: 30
        color: "transparent"
        Label {
            text: qsTr("参数设置")
            color: "steelblue"
            font.bold: true
//            anchors.centerIn: parent  // 设置文本居中
        }
    }
    Rectangle {
        y: 30
        color: "steelblue"
        width: parent.width
        height: 3
    }

    ColumnLayout {
        RowLayout {

        }
        RowLayout {
            GridDemos {
                x: 17
                y: 45
            }
        }
    }

}
