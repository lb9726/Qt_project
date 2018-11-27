import QtQuick 2.3

Item {                                                  // QML通用的根元素
    id: window
    width: 600
    height: 240
    visible: true
    Rectangle{
        x:10//这里的坐标是相对于它的父窗口，也就是Window
        y:10
        width: 100;
        height: 100;//一定要指定出大小才能显示
        visible: true
        antialiasing: true;//抗锯齿，默认就是开启的
        border.width: 10;
        border.color: "red";//边框的一些属性
        color: "blue"//内部的颜色
        gradient: Gradient{//颜色渐变
            GradientStop { position: 0.0; color: "lightsteelblue" }
            GradientStop { position: 1.0; color: "blue" }
        }
        radius: 10;//圆角
        clip:true//这一属性设置表示如果他的子类超出了范围，那么就剪切掉，不让他显示和起作用
        Rectangle{
            id:son
            x:50;
            y:50;
            width: 300;
            height: 100;
            color: "gray";
        }
    }
}
