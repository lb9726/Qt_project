import QtQuick 2.0
import QtGraphicalEffects 1.0                // (a)
Rectangle {                                  // 矩形作为图像显示区
    width: animg.width
    height: animg.height

    AnimatedImage {                          // 显示GIF图像元素
        id: animg
        source: "images/insect.gif"
    }

    BrightnessContrast {                     // (b)
        id: bright
        anchors.fill: animg
        source: animg
    }

    SequentialAnimation {                    // 定义串行组合动画
        id: imgAnim
        NumberAnimation {                    // 用动画调整亮度
            target: bright
            property: "brightness"           // (c)
            to: -0.5                         // 变暗
            duration: 3000
        }
        NumberAnimation {
            target: bright                   // 用动画设置对比度
            property: "contrast"             // (d)
            to: 0.25
            duration: 2000
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            imgAnim.running = true            // 单击图像开启动画
        }
    }
}
/*
(a) import QtGraphicalEffects 1.0 :QML 的图形特效元素类型都包含在QtGraphicalEffects库中，编程时需要
使用到该模块处理图像，都要在QML文档开头写上这一句声明，以导入特效元素库。
（b） BrightnessContrast {…}：BrightnessContrast 是一个特效元素，功能是设置源元素的亮度和对比度。它有
一个属性source指明了源元素，源元素一般都是一个Image或AnimatedImage类型的图像
（c） property：“contrast": constrast 也是BrightnessContrast元素的属性，用于设置源元素的宽度，由最暗
到最亮对应的取值范围为：-1.0 ～ 1.0，默认值为0.0 （对应图像的本来亮度）。本例用动画渐变到目标值-0.5,
在视觉上呈现较暗的效果
（d)property：”contrast“： constrast 也是BrightnessContrast元素的属性，用于设置源元素的对比度,
由最弱到最强对应的取值范围为-1.0 ～ 1.0，默认值为0.0（对应图像本来的对比度)。0.0 ～ -1.0 的对比度是以线性递减的，
而0.0 ～ 1.0 的对比度则呈非线性增强，且越接近1.0增加曲线越陡峭，以致达到很高的对比效果。
本例用动画将对比度逐渐调节到0.25,视觉上能十分清楚地显示花蕾上的昆虫。
*/
