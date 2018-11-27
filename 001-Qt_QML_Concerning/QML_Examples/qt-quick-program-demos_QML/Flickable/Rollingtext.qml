import QtQuick 2.0

Item {
    id: retScroll   //x: 290 y: 684
    width: 734
    height: 83
    Text {
        id:idtext
        height:parent.height
        text:"欢迎光临上海贝思特"
        color:"white"
        font.bold: true          // 文字加粗
        font.pixelSize: 24       // 文字大小
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

