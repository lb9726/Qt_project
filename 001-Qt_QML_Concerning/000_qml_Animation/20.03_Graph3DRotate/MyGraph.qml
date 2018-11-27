import QtQuick 2.0

Rectangle {                            // 矩形作为图像显示区
    /* 矩形宽度，高度皆与图像尺寸吻合*/
    width: animg.width
    height: animg.height
    transform: Rotation {             // (a)
        origin.x : 110/*animg.width / 2*/
        origin.y : 200/*animg.height / 2*/
//        axis {
//            x: 0
//            y: 1
//            z: 0                      // 绕y轴转动
//        }

        NumberAnimation on angle {    // 定义角度angle上的动画
            from: 0
            to: 360
            duration: 20000
            loops: Animation.Infinite
        }
    }
        AnimatedImage {
            id: animg
            source: "images/bee.gif"  // 照片路径
        }

}
/*
(a) transform：Rotation {…}： transform属性，需要指定一个Transform 类型元素的列表。
在QML中可用的Transform类型有3个，即Rotaion，Scale和Translate，分别用来进行旋转，缩放和平移。
这些元素还可以通过专门的属性来进行更加高级的变换位置。其中，Rotation 提供了坐标轴和原点属性，
坐标轴有axis.x,axis.y和axis.z, 分别代表x轴，y轴和z轴，因此可以实现3D效果。原点由origin.x 和
origin.y 来指定。对于典型的3D旋转，既要指定原点，也要指定坐标轴。……，使用angle属性指定顺时针旋转的角度

（b）AnimatedImage {……}：AnimatedImage扩展了Image元素的功能，可以用来播放包含一系列帧的图片动画，
如GIF文件。当前帧和动画总长度等信息可以使用currentFrame和frameCount 属性来获取，还可以通过改变
playing和paused的值来开始，暂停和停止动画。
*/
