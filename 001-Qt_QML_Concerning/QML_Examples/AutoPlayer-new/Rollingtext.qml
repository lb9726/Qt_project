import QtQuick 2.0

Rectangle {
    width: 1024
    height: 50
    Text {
        id:idtext
        height:parent.height
        text:"Shanghai BST Electric Co.,ltd . Add：Da-maiwan Industrial Zone,Hangtou,Pudong New Area,Shanghai 201316,China"
        color:"white"
        font.bold: true          // 文字加粗
        font.pixelSize: 24       // 文字大小
        verticalAlignment: Text.AlignVCenter      // 水平居中
//        anchors.centerIn: parent;  // 这样设置将没有动画
        SequentialAnimation on x {
        loops: Animation.Infinite
            PropertyAnimation {
                from:parent.width
                to: -idtext.width
                duration: 40000      // 滚动的文字运行的时间长度
            }
       }
    }
}

