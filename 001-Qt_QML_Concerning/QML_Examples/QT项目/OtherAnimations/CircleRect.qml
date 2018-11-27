import QtQuick 2.0

Rectangle {
    width: 80
    height: 80

    ColorAnimation on color{                         // (a)
        from: "blue"
        to: "aqua"
        duration: 10000
        loops: Animation.Infinite                   //无限循环
    }

    RotationAnimation on rotation {                 // (b)
        from: 0
        to: 360
        duration: 10000
        direction: RotationAnimation.Clockwise     // 顺时针方向旋转
        loops: Animation.Infinite                  // 无限循环
    }

    NumberAnimation on radius {                    // (c)
        from: 0
        to: 40
        duration: 10000
        loops: Animation.Infinite                  //无限循环
    }

    PropertyAnimation on x {
        from: 50
        to: 500
        duration: 10000
        loops: Animation.Infinite                 //无限循环
        easing.type: Easing.InOutQuad             // 先加速，后减速
    }
}
/*
(a)ColorAnimation on color{……}：ColorAnimation 动画元素允许颜色值设置from和to属性，
这里设置from为blue，to为aqua，即矩形颜色从蓝色逐渐变为水绿色。

(b)RotationAnimation on rotation {……}： RotationAnimation 动画元素允许设定图形旋转的方向，
本例通过指定from 和 to 属性，使得矩形360度旋转。设direction属性RotationAnimation.Clockwise 表示顺时针方向旋转；
如果设为RotationAnimation.Counterclockwise 则表示逆时针旋转

(c)NumberAnimation on radius {……}： NumberAnimation 动画元素是专门应用于数值类型的值改变的属性动画元素，
本例用它来改变矩形的圆角半径值。因矩形长宽均为80，将圆角设为40可使得矩形呈现为圆形，
故radius属性值从0变化到40的动画效果是：矩形的四个棱角逐渐磨圆最终彻成为一个圆形。
*/
