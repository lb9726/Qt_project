import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
//import QtQuick.Layouts 1.1
import QtMultimedia 5.7
//import QtMultimedia 5.5
ApplicationWindow {
    id:id_mediaScreenHelper
    visible: true
    property int windowsHeight: 650
    property int windowsWidth: 1000

    property bool resetEnabled: false  //用户是否使能了Reset功能
    property alias multiMediaView: id_multiMediaView
    property alias parameterView: id_parameterView

    maximumHeight:windowsHeight
    maximumWidth:windowsWidth

    minimumHeight:windowsHeight
    minimumWidth:windowsWidth

    title: qsTr("MediaScreenHelper") + "  V1.6.0"

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        MultiMedia{
            id:id_multiMediaView
            clip: true
        }

        Parameter{
            id:id_parameterView
            clip: true
        }

        MakeDisk{
            clip: true
        }
    }

    background: Image{
        anchors.fill: parent
        opacity: 0.6
        source: "qrc:///image/background.png"
    }

    footer: TabBar {
        id: tabBar
        height: 50
        enabled: !resetEnabled
        currentIndex: swipeView.currentIndex
        TabButton {
            text: qsTr("1 Multimedia Update")
            Rectangle{
                width: 2
                height: parent.height*2/3
                anchors.verticalCenter: parent.verticalCenter
                color: "green"
                opacity: 0.6
                anchors.right: parent.right
            }
        }
        TabButton {
            text: qsTr("2 Parameter Configure")
            Rectangle{
                width: 2
                height: parent.height*2/3
                anchors.verticalCenter: parent.verticalCenter
                color: "green"
                opacity: 0.6
                anchors.right: parent.right
            }
        }
        TabButton {
            text: qsTr("3 Make Disk")
        }
    }
}
