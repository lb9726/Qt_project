import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.2

Item {
    property int area1_width: 432
    property int area3_width: 173
    property int area1_height: 460
    property int area4_height: 144
    id: testare3_Item
    anchors.fill: parent
    ColumnLayout {
        id: wholeColumn
        anchors.fill: parent
        RowLayout {
            id: mulmedia_row
            width: 173
            height: 40
            Item {
                id: labelItem
                anchors.fill: parent
                height: updateMedia_id.height
                Label {
                    id: updateMedia_id
                    text: qsTr("多媒体更新")
                    anchors.centerIn: parent
                }
            }
        }
        RowLayout {
            id: showMode_id
            width: 173
            Item {
                anchors.centerIn: parent
                id: row2_Item
//                anchors.fill: parent
                ColumnLayout {
                    id: row2_column
                    width: 173
//                    anchors.centerIn: parent
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
        }
        RowLayout {
            id: videoAndpicture
        }
        RowLayout {
            id: select_videoOrPic
        }
    }
    ColumnLayout {

    }

    Component.onCompleted: {
        console.log("testare3_Item = ", testare3_Item.width, testare3_Item.height)
        console.log("wholeColumn = ", wholeColumn.width, wholeColumn.height)
        console.log("mulmedia_row = ", mulmedia_row.width, mulmedia_row.height)
        console.log("showMode_id = ", showMode_id.width, showMode_id.height)
        console.log("videoAndpicture = ", videoAndpicture.width, videoAndpicture.height)
        console.log("select_videoOrPic = ", select_videoOrPic.width, select_videoOrPic.height)
        console.log("labelItem = ", labelItem.width, labelItem.height)
    }
}
