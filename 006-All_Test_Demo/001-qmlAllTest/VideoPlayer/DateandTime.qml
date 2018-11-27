import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
//import lb2616.tools.Protocoldeal 1.0
// 主画面坐标为：x: 779; y: 27

Rectangle {
    id: rec
//    color: "transparent"
    color: "blue"
    property string timeanddate : ""
    width: 123
    height: 75
//    Timer {
//        interval: 1000; running: true; repeat: true
//        onTriggered: time.text = Qt.formatDateTime(new Date(), "hh:mm")
//    }

//    Timer {
//        interval: 1000; running: true; repeat: true
//        onTriggered: date.text = Qt.formatDateTime(new Date(), "dd.M.yyyy")
//    }
//    Protocoldeal {
//        id: pro
//    }

    Text { // x: 884; y: 45
        x: 0; y: 0
        id: time
        text: Qt.formatDateTime(new Date(), "hh:mm")
        color: "white"
        font.family: "KONE Information_v12"
        font.pixelSize: 56
        anchors.bottom: date.top
        anchors.top: rec.top
    }
    Text {
        id: date
        text: Qt.formatDateTime(new Date(), "dd.M.yyyy")
        color: "white"
//        width: time.width
        font.family: "KONE Information_v12"
        font.pixelSize: 30
        anchors.bottom: rec.bottom
//        anchors.right: rec.right
        anchors.rightMargin: 19
//        anchors.topMargin: 25
        anchors.horizontalCenter: time.horizontalCenter  // 与下面的日期水平居中对齐
    }

//    Connections: {
//        target: pro
//        onSendTimerChanged:
//            time.text = tim;
//            date.text = dat;
//    }
}

