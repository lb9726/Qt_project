import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
// 主画面坐标为：x: 779; y: 27

Item {
    id: rec
    property string timeanddate : ""
    width: 210
    height: 70
    Timer {
        interval: 1000; running: true; repeat: true
        onTriggered: time.text = Qt.formatDateTime(new Date(), "hh:mm:ss")
    }

    Timer {
        interval: 1000; running: true; repeat: true
        onTriggered: date.text = Qt.formatDateTime(new Date(), "MM-dd-yyyy")
    }

    Text { // x:779; y:27
        id: time
        text: Qt.formatDateTime(new Date(), "hh:mm:ss")
        color: "white"
        font.bold: true
        font.pixelSize: 28
        anchors.bottom: date.top
        anchors.horizontalCenter: date.horizontalCenter  // 与下面的日期水平居中对齐
    }
    Text {
        id: date
        text: Qt.formatDateTime(new Date(), "MM-dd-yyyy")
        color: "white"
        font.bold: true
        font.pixelSize: 28
        anchors.bottom: rec.bottom
        anchors.right: rec.right
    }
}

