import QtQuick 2.0

Rectangle {
    width: 390
    height: 517
    color: "black"

    Item {
        x: 20
        y: 13
        width: 263
        height: 29
        Text {
            id: title_text
            text: qsTr("Shanghai Mart")
            color: "white"
            font.pixelSize: 30
        }
    }

    Item {
        x: 307
        y: 13
        width: 82
        height: 24
        Text {
            id: time_text
            text: qsTr("00:00")
            color: "white"
            font.pixelSize: 30
            verticalAlignment: Text.AlignVCenter      // 水平居中
        }
    }

    Item {
        x: 307
        y: 47
        width: 82
        height: 24
        Text {
            x: 1
            id: date_text
            text: qsTr("DD:MM:YYYY")
            color: "white"
            font.pixelSize: 14
            verticalAlignment: Text.AlignVCenter      // 水平居中
        }
    }

    Item {
        id: showImage
        x: 0
        y: /*194*/ 174
        width: 389
        height: 296
        Image {
            id: image_back
            width: parent.width
            height: parent.height
            anchors.centerIn: parent
            source: "qrc:///images/Lighthouse.jpg"
        }
    }
    Item {
        id: scroll_item   //x: 290 y: 684
        x: 0
        y: /*488*/ 468
        width: 389
        height: 29 + 20
        clip: true
        Text {
            id: scroll_text
            height:parent.height
            text:"Welcome to Shanghai BST"
            color:"white"
            font.family: "KONE Information_v12"
            font.pixelSize: 20       // 文字大小
            verticalAlignment: Text.AlignVCenter      // 水平居中
            SequentialAnimation on x {
            loops: Animation.Infinite
                PropertyAnimation {
                    from: scroll_item.width
                    to:  -scroll_text.width
                    duration: 12000      // 滚动的文字运行的时间长度
                }
           }
        }
    }
}
