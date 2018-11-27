import QtQuick 2.3

Rectangle {
    property alias mouseArea: mouseArea

    width: 200           // (a)
    height: 200

    MouseArea {
        id: mouseArea
        anchors.fill: parent
    }

    Flow {               // (b)
        anchors.fill: parent
        anchors.margins: 15    //元素与窗口左上角边距为15像素
        spacing: 5
        //以下添加被Flow定位的元素成员
        RedRectangle { }
        BlueRectangle { }
        GreenRectangle { }
    }
}
/*
（a）width: 200;height: 200 ：为了令Flow正确工作并演示出其实用效果，需要指定元素显示区的宽度和高度

（b)Flow {……}:顾名思义，Flow会将其元素成员以流的形式显示出来，它既可以从左向右横向布局，也可以从上向下纵向布局，或反之。
但与Row,Column等定位器不同的是，添加到Flow里的元素，会根据显示区（窗体)尺寸变化动态地调整其布局。以本程序为例，
初始运行时窗体狭窄，无法横向编排元素，故三个矩形都纵向排列，在用鼠标将窗体拉宽过程中，其中矩形由纵排逐渐变成横排显示
*/
