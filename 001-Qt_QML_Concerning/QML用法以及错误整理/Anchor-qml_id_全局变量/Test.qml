import QtQuick 2.3

Rectangle {
    property alias mouseArea: mouseArea
    /* 定义属性别名 */                                      // (a)
    property alias chgRect1: changingRect1                //矩形changingRect1属性别名
    property alias chgRect2: changingRect2                //矩形changingRect2属性别名
    property alias rRect: redRect                         //红矩形redRect属性别名

    width: 360
    height: 360

    MouseArea {
        id: mouseArea
        anchors.fill: parent
    }

    /*使用Achor对三个矩形元素进行横向布局*/      // (b)
    BlueRectangle {                         // 蓝矩形
        id: blueRect
        anchors.left: parent.left           //与窗口左锚线锚定
        anchors.top: parent.top             //与窗口顶锚线锚定
        anchors.leftMargin: 25              //左锚边距 (即与窗口左边距)
        anchors.topMargin: 25               //顶锚边距 (即与窗口顶边距)
    }

    GreenRectangle {                        // 绿矩形
        id: greenRect
        anchors.left: blueRect.right        //绿矩形左锚线与蓝矩形的右锚线锚定
        anchors.top: blueRect.top           //绿矩形顶锚线与蓝矩形的顶锚线锚定
        anchors.leftMargin: 40              //左锚边距（即与蓝矩形的间距)
    }


    RedRectangle {                          // 红矩形
        id: redRect
        anchors.left: greenRect.right       //红矩形左锚线与绿蓝矩形的右锚线锚定
        anchors.top: greenRect.top          //红矩形顶锚线与绿矩形的顶锚线锚定
        anchors.leftMargin: 40              //左锚边距（即与绿矩形的间距)
    }

    //对比测试Anchor的性质                   // (c)
    RedRectangle {
        id: changingRect1
        anchors.left: parent.left         //矩形changingRect1初始与窗体左锚线锚定
        anchors.top: blueRect.bottom
        anchors.leftMargin: 25
        anchors.topMargin: 25
    }

    RedRectangle {                        // 红矩形
        id: changingRect2
        anchors.left: parent.left         // changingRect2 与 changingRect1 左对齐
        anchors.top: changingRect1.bottom
        anchors.leftMargin: 25
        anchors.topMargin: 20
    }
    // 复用按钮
    Button {
        x: 202
        width: 95; height: 35
        anchors.rightMargin: 0              // (d)
        anchors.right: redRect.right
        anchors.top: changingRect2.bottom
        anchors.topMargin: 10
    }
}
/*

(a)定义属性别名：这里定义矩形changingRect1，changingRect2以及redRect的别名，目的是在按钮组件的源文件(外部QML文档)中
能访问这几个元素,以便测试它们的锚定特性

(b) 使用Anchor对三个矩形元素进行横向布局:这段代码使用已经定义的3个现成矩形元素，通过分别设置anchor.left，anchor.top，
anchor.leftMargin，anchors.topMargin的锚属性，对他们进行从左到右的布局，这与之前讲过的Row的布局作用是一样的。读者还
可以修改其他锚属性以尝试更多的布局效果

(c)对比测试Anchor的性质：锚属性还可以在程序运行中通过代码设置来动态地改变，为了对比，本例设计用两个相同的红色矩形，初始
它们都与窗体左锚线锚定（对齐），然后改变右锚属性观察它们的行为

(d)width：95；height: 35： 按钮组件原定义尺寸为width：100；height：62”，复用时可以重新定义它的尺寸属性以使程序界面更美观
。新属性值会”覆盖“ 原来的属性值，就像面向对象的“继承”一样提高了灵活性

*/
