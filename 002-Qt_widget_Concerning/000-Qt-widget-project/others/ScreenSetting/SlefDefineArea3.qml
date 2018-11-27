import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.2

Item {
    id: slefdefineArea3
    anchors.fill: parent
    Rectangle {
        id: labelItem
        width: parent.width
        color: "transparent"
        y: 20
        height: updateMedia_id.height
        Label {
            id: updateMedia_id
            text: qsTr("多媒体更新")
            anchors.centerIn: parent
        }
    }

    Rectangle {
        id: radioButtonItem
        width: parent.width
        height: 66
        color: "transparent"
        y: 45
        ColumnLayout {
            anchors.centerIn: parent
            id: radioButtonColumnLayout1
            RadioButton {
                id: normal_show
                checked: true
                text: qsTr("正常显示")
            }
            RadioButton {
                id: fullscreen_show
                text: qsTr("全屏显示")
            }
        }
    }

    Rectangle {
        id: radioButton_video
        width: parent.width
        y: 120
        RowLayout {
            spacing: 30
            RadioButton {
                id: select_video
                text: qsTr("视频")
                checked: true
            }
            RadioButton {
                id: select_picture
                text: qsTr("图片")
            }
        }
    }
    Component.onCompleted: {
        console.log("slefdefineArea3 = ", slefdefineArea3.width, slefdefineArea3.height)
        console.log("labelItem = ", labelItem.width, labelItem.height)
        console.log("radioButtonItem = ", radioButtonItem.width, radioButtonItem.height)
        console.log("radioButtonColumnLayout1 = ", radioButtonColumnLayout1.width, radioButtonColumnLayout1.height)
        console.log("normal_show = ", normal_show.width, normal_show.height)
        console.log("radioButton_video = ", radioButton_video.width, radioButton_video.height)
        console.log("select_video = ", select_video.width, select_video.height)
        console.log("select_picture = ", select_picture.width, select_picture.height)
    }
}
