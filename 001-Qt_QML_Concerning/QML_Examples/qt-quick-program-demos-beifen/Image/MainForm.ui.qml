import QtQuick 2.3

Rectangle {
    property alias mouseArea: mouseArea

    width: 860
    height: 860

    MouseArea {
        id: mouseArea
        anchors.fill: parent
    }
/*
    Text {
        anchors.centerIn: parent
        text: "Hello World"
    }
*/
    Image {
        // 图像在窗口的位置坐标
        x: 80
        y: 20
        // 宽和高均为原图的1/4
        width: 1024/4; height: 720/4    //(a)
        source: "images/牧场人家.jpg"
        fillMode: Image.PreserveAspectCrop  // (b)
        clip: true    // 避免所要渲染的图片超出元素范围
    }
}
/*

(a) width: 1024/4; height: 720/4 :Image的width 和 height 属性用来设定图元的大小，
如果没有设置，Image会使用图片本身的尺寸；如果设置了，那么图片就会拉伸来适应这个尺寸。
本例设置它们均为原图的1/4，为的是使其缩小后不变形

(b) fillMode: Image.PreserveAspectCrop ： fillMode 属性设置图片的填充模式，它支持
Image.Stretch(拉伸)，Image.PreserAspectFit(等比缩放),Image.PreserveAspectCrop（
等比缩放，最大填充Image，必要时裁剪图片),Image.Tile（在水平和垂直两个方向平铺,就像
贴瓷砖那样），Image.TileVertically(垂直平铺)，Image.TileHorizontally（水平平铺）,
Image.Pad（保持图片原样不做变换)等模式

 */
