import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.1

Item {
    Text {
        id: str
        text:qsTr("System is updating ,please wait patiently....")
        font.bold: true          // 文字加粗
        font.pixelSize: 24       // 文字大小
        anchors.centerIn: parent
    }

    ProgressBar
    {
        id: curProgress
        anchors.left: str.left
        anchors.top: str.bottom
        width: str.width
        height: 25
        minimumValue: 0
        maximumValue: 100
        value: 100

        style: ProgressBarStyle{
            id: progressStyle;
            background: Rectangle{
                border.width: 1;
                border.color: control.hovered?"green":"gray";
                color:"lightgray";
            }
            progress: Rectangle{
                color:"#208020";
            }
        }

        Text {
            color: "red";
            text:curProgress.value + "%";
            anchors.centerIn: parent;
        }
    }
}

