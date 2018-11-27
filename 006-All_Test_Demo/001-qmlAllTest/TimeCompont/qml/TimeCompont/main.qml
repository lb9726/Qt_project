import QtQuick 1.1
import "../Common"

Rectangle {
    width: 480
    height: 360
    color: "lightblue"

    Row {
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 150
        Text {
            id: startTime
            font.pixelSize: 30
            text: "00:00"
        }

        Text {
            id: endTime
            font.pixelSize: 30
            text: "00:00"
        }
    }

    Row {
        anchors.centerIn: parent
        spacing: 30
        TimeCompont {
            title: "开始时间"
            onTimeSig:
                startTime.text = hourIndex + ":" + minIndex
        }

        TimeCompont {
            title: "结束时间"
            onTimeSig:
                endTime.text = hourIndex + ":" + minIndex
        }
    }
}
