import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
ApplicationWindow {
    id:appwindow
    visible: true
    property int windowsHeight: 570
    property int windowsWidth: 990

    property bool resetEnabled: false
//    property alias selectfile_id2: selectfile_id2
    //用户是否使能了Reset功能
//    property alias multiMediaView: id_multiMediaView
//    property alias parameterView: id_parameterView

//    maximumHeight:windowsHeight
//    maximumWidth:windowsWidth

    minimumHeight:windowsHeight
    minimumWidth:windowsWidth

    title: qsTr("MediaScreenHelper") + qsTr("  V1.6.0")

    background:Image{
        anchors.fill: parent
        opacity: 0.8
        source: "qrc:///background1.jpg"
    }

//    TestLayout {

//    }
//    LayoutTest {

//    }
//    ShowDisplay {

//    }
//    ResultDisplay {
//        x:2
//        y:2
//    }
//    GroupBoxDemo {

//    }
    Rectangle {
        width: parent.width
        height: 30
        color: "transparent"
        Label {
            text: qsTr("参数设置")
            anchors.centerIn: parent  // 设置文本居中
        }
    }
    GridDemos {
        x: 17
        y: 45
    }
}
