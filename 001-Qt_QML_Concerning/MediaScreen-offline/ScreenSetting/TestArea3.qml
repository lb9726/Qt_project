import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.2

Item {
    id: testare3_Item
    anchors.fill: parent
    ColumnLayout {
        id: wholeColumn
        anchors.fill: parent
        RowLayout {
            id: mulmedia_row
            width: parent.width
            Item {
                id: labelItem
                anchors.fill: parent
                height: updateMedia_id.height
                Label {
                    id: updateMedia_id
                    text: qsTr("多媒体更新")
//                    width: parent.width
                    anchors.centerIn: parent
                }
            }
        }
        RowLayout {
            id: showMode_id
        }
        RowLayout {
            id: videoAndpicture
        }
        RowLayout {
            id: select_videoOrPic
        }
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
