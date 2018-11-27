import QtQuick 2.0

Rectangle {
    width: 512
    height: 384
    color: "black"

    Item {
        x: 174
        y: 17
        width: 250
        height: 32
        Text {
            id: title_text
            text: qsTr("Shanghai Mart")
            color: "white"
            font.pixelSize: 30
        }
    }

    Item {
        x: 439
        y: 17
        width: 74
        height: 26
        Text {
            id: time_text
            text: qsTr("00:00")
            color: "white"
            font.pixelSize: 30
            verticalAlignment: Text.AlignVCenter      // 水平居中
        }
    }

    Item {
        x: 439
        y: 47
        width: 74
        height: 20
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
        x: 175
        y: 70
        width: 337
        height: 255
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
        x: 175
        y: 325
        width: 337
        height: 64
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
