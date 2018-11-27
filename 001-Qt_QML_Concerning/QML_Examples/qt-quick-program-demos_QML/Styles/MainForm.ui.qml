import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.3                      // 导入Qt Quick 控件样式库

Item {                                                  // QML通用的根元素
    id: window
    width: 600
    height: 240

    property int columnWidth: window.width / 5          // 公共属性ColumnWidth 用于设置控件列宽

    GridLayout {                                                // 网格布局
        rowSpacing: 12                                          //  行距
        columnSpacing: 30                                       //  列距
        anchors.top: parent.top                                 // 与主窗体顶端对齐
        anchors.horizontalCenter: parent.horizontalCenter       // 在主窗体居中
        anchors.margins: 30                                     // 锚距30
        Button {
            text: "标准按钮"
            implicitWidth: columnWidth                          // (a)
        }
        Button {                                                // 设置样式属性的Button 控件
            text: "样式属性"
            style: ButtonStyle {                                // 样式属性
                background: BorderImage {
                    source: control.pressed ? "images/button-pressed.png" : "images/button.png"
                    border.left: 4; border.top: 4
                    border.right: 4; border.bottom: 4
                }
            }
            implicitWidth: columnWidth
        }
        Button {                                         // 应用样式代理的Button控件
            text: "样式代理"
            style: buttonStyle                           // buttonStyle 为样式代理名
            implicitWidth: columnWidth
        }
        TextField {                                     // 标准TextField控件
            Layout.row: 1                               // 指定在GridLayout中的行号为1(第2行)
            implicitWidth: columnWidth
        }
        TextField {
            style: TextFieldStyle {
                background: BorderImage {                // 设置背景图片为textfield.png
                    source: "images/textfield.png"
                    border.left: 4; border.right: 4; border.top: 4; border.bottom: 4
                }
            }
            implicitWidth: columnWidth
        }
        TextField {                                     // 应用样式代理的TextField控件
            style: textFieldStyle                       // textFieldStyle 为样式代理名
            implicitWidth: columnWidth
        }
        Slider {                                       // 标准Slider控件
            id: slider1
            Layout.row: 2                              // 指定在GridLayout中的行号为2
            value: 0.5                                 // 初始值
            implicitWidth: columnWidth
        }
        Slider {
            id: slider2
            value: 0.5
            implicitWidth: columnWidth
            style: SliderStyle {                       // 样式属性
                groove: BorderImage {                  // (c)
                    height: 6
                    border.top: 1
                    border.bottom: 1
                    source: "images/progress-background.png"
                    border.left: 6
                    border.right: 6
                    BorderImage {
                        anchors.verticalCenter: parent.verticalCenter
                        source: "images/progress-fill.png"
                        border.left: 5; border.top: 1
                        border.right: 5; border.bottom: 1
                        width: styleData.handlePosition                // 宽度至手柄（滑块）的位置
                        height: parent.height
                    }
                }
                handle: Item {                      // (d)
                    width: 13
                    height: 13
                    Image {
                        anchors.centerIn: parent
                        source: "images/slider-handle.png"
                    }
                }
            }
        }
        Slider {                                        // 应用样式代理的Slider控件
            id: slider3
            value: 0.5
            implicitWidth: columnWidth
            style: sliderStyle                         // SliderStyle 为样式代理名
        }
        ProgressBar {                                  /*标准ProgressBar控件*/
            Layout.row: 3                              // 指定在GridLayout中的行号为3(第4行)
            value: slider1.value                       // 进度值设为与滑动条同步
            implicitWidth: columnWidth
        }
        /*以下两个为应用不同样式代理的ProgressBar控件*/
        ProgressBar {
            value: slider2.value
            implicitWidth: columnWidth
            style:progressBarStyle                    // 应用样式代理 ProgressBarStyle
        }
        ProgressBar {
            value: slider3.value
            implicitWidth: columnWidth
            style: progressBarStyle2                  // 应用样式代理 ProgressBarStyle2
        }
    }

    /*以下为定义各种样式代理的代码*/
    property Component buttonStyle: ButtonStyle {
        /*Button 控件所使用的样式代理*/
        background: Rectangle {
            implicitHeight: 22
            implicitWidth: columnWidth
            // 按钮被压下或获得焦点时变色
            color: control.pressed ? "darkGray" : control.activeFocus ? "#cdd" : "#ccc"
            antialiasing: true                              // 平滑边缘反锯齿
            border.color: "gray"
            radius: height / 2                              // 圆角形
            Rectangle {
                anchors.fill: parent
                anchors.margins: 1
                color: "transparent"                       // 透明色
                antialiasing: true
                visible: !control.pressed                  // 在按钮未被按下时可见
                border.color: "#aaffffff"
                radius: height / 2
            }
        }
    }

    property Component textFieldStyle: TextFieldStyle {
        // TextField 控件所使用的样式代理
        background: Rectangle {                          // 文本框背景为矩形
            implicitWidth: columnWidth
            color: "#f0f0f0"
            antialiasing: true
            border.color: "gray"
            radius: height / 2
            Rectangle {
                anchors.fill: parent
                anchors.margins: 1
                color: "transparent"
                antialiasing: true
                border.color: "#aaffffff"
                radius: height / 2
            }
        }
    }

    property Component sliderStyle: SliderStyle {
        // slider控件所使用的样式代理
        /*handle:*/ Rectangle {   // 定义矩形作为滑块
            width: 18
            height: 18
            color: control.pressed ? "darkGray" : "lightGray"
            // 按下时灰度改变
            border.color: "gray"
            antialiasing: true
            radius: height / 2        // 滑块呈圆形
            Rectangle {
                anchors.fill: parent
                anchors.margins: 1
                color: "transparent"
                antialiasing: true
                border.color: "#eee"
                radius: height / 2
            }

        }
        groove: Rectangle {             // 定义滑动条的横槽
            height: 8
            implicitWidth: columnWidth
            implicitHeight: 22
            antialiasing: true
            color: "#ccc"
            border.color: "#777"
            radius: height / 2         // 使得滑动条横槽两端有弧度（外观平滑）
            Rectangle {
                anchors.fill: parent
                anchors.margins: 1
                color: "transparent"
                antialiasing: true
                border.color: "#66ffffff"
                radius: height / 2
            }
        }
    }
    // progressBarStyle:  控件所使用的样式代理1
    property Component progressBarStyle: ProgressBarStyle {
        background: BorderImage {
            source: "images/progress-background.png"
            border.left: 2; border.top: 2
            border.right: 2; border.bottom: 2
        }
        progress: Item {
            clip: true
            BorderImage {
                anchors.fill: parent
                anchors.rightMargin: (control.value < control.maximumValue) ? -4 : 0
                source: "images/progress-fill.png"
                border.left: 10; border.right: 10
                Rectangle {
                    width: 1
                    color: "#a70"
                    opacity: 0.8
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 1
                    anchors.right: parent.right	
                    visible: control.value < control.maximumValue          // 进度条未到头时始终可见
                    anchors.rightMargin: -parent.anchors.rightMargin       // 两者锚定互补达到进度效果
                }
            }
        }
    }

    // progressBarStyle:  控件所使用的样式代理2
    property Component progressBarStyle2: ProgressBarStyle {
        background: Rectangle {
            implicitWidth: columnWidth
            implicitHeight: 24
            color: "#f0f0f0"
            border.color: "gray"
        }
        progress: Rectangle {
            color: "#ccc"
            border.color: "gray"
            Rectangle {
                color: "transparent"
                border.color: "#44ffffff"
                anchors.fill: parent
                anchors.margins: 1
            }
        }
    }
}

/*
(a)implicitWidth: columnWidth: QML根元素Item有一个implicitWidth 属性，他设定了对象的隐式宽度
作为其实际的宽度。所有QML可视元素以及QtQuick 控件都继承了implicitWidth属性，本例用它保证了个控件
的宽度始终都维持在ColumnWidth（主窗口宽度的1/5），并随着窗口大小的改变自动调节尺寸。

（b） background：BorderImage {…}： 设置控件所用的背景图，图片来源即之前载入项目中的资源。这里
条件运算符设置当按钮按下时，背景显示button-pressed.png(一个深灰色矩形）；而未按下时则显示button.png(
颜色较浅的矩形），由此实现了单击时按钮颜色的变化

（c）groove：BorderImage {…}：groove 设置滑动条横槽的外观，这里外层BorderImage所用图片为Progress-fill.png
设置了橙黄色充满状态的外观，其宽度与滑块所在的位置一致。

（d）handle：Item {…}：handle 定义了滑块的样子，这里采用图片Slider-handle.png展示滑块的外观

（e）Progress：Item {…}：progress 设置进度条的外观，用progress-fill.png 定制进度条已填充部分，有定义了一个
Rectangle子元素来显示其未充满的部分，通过与父元素BorderImage的锚定和可见性控制巧妙地呈现进度条的外观。
*/

