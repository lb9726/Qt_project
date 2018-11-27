import QtQuick 2.0

Item {
    id: retScroll   //x: 290 y: 684
    width: 675
    height: 81
    Text {
        id:idtext
        height:parent.height
        text:"Welcome to Shanghai BST"
        color:"white"
        font.family: "KONE Information_v12"
        font.pixelSize: 32       // 文字大小
        verticalAlignment: Text.AlignVCenter      // 水平居中
//        anchors.centerIn: parent;  // 这样设置将没有动画
        SequentialAnimation on x {
        loops: Animation.Infinite
            PropertyAnimation {
                from:0
                to: retScroll.width
                duration: 12000      // 滚动的文字运行的时间长度
            }
       }
    }
}

