import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import lb2616.tools.RGBGame 1.0

Item {
    width: 395; height: 360;

    Text {
        id: timeLabel
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10
        font.pixelSize: 26
    }

    RGBGame {
        id: rgbGame
        color: Qt.blue
    }

    Rectangle {
        id: colorRect
        anchors.centerIn: parent
        width: 200
        height: 200
        color: "yellow"
    }

    Button {
        id: start
        text: "start"
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        onClicked: rgbGame.start()
    }

    Button {
        id: stop
        text: "stop"
        anchors.left: start.right
        anchors.leftMargin: 5
        anchors.bottom: start.bottom
        onClicked: rgbGame.stop()
    }

    function changeAlgorithm(button, algorithm)
    {
        switch(algorithm)
        {
            case 0:
                button.text = "RandomRGB";
                break;
            case 1:
                button.text = "RandomRed"
                break;
            case 2:
                button.text = "RandomGreen"
                break;
            case 3:
                button.text = "RandomBlue"
                break;
            case 4:
                button.text = "LinearIncrease"
                break;
        }
    }

    Button {
        id: colorAlgorithm
        text: "RandomRGB"
        anchors.left: stop.right
        anchors.leftMargin: 5
        anchors.bottom: start.bottom

        onClicked: {
            var algorithm = (rgbGame.algorithm() + 1)%5
            changeAlgorithm(colorAlgorithm, algorithm)
            rgbGame.setAlgorithm(algorithm)
        }
    }

    Button {
        id: quit
        text: "quit"
        anchors.left: colorAlgorithm.right
        anchors.leftMargin: 5
        anchors.bottom: start.bottom
        onClicked: Qt.quit()
    }

    Component.onCompleted: {
        rgbGame.color = Qt.rgba(0, 180, 120, 255)
        rgbGame.setAlgorithm(rgbGame.LinearIncrease)
        changeAlgorithm(colorAlgorithm, rgbGame.algorithm())
    }

    Connections {
        target: rgbGame
        onCurrentTime: {
            timeLabel.text = strTime
            timeLabel.color = rgbGame.timeColor
        }
    }

    Connections {
        target: rgbGame
        onColorChanged: colorRect.color = color
        // 此处的color正是对应的类的 signal void colorChanged(const QColor &color)的参数，可以在qml中使用，修改.h中的参数，则qml发生错误
    }
}
/*
main.qml 中使用 RGBGame 构造了一个对象,id 为 rgbGame ,这样就可以借助 id 来
访问 C++类对象了 , 这里还定义了一个 changeAlgorithm 函数,参数 button 和 algorithm
拥有动态类型。
界面由三部分组成。顶部是一个 Text ,用来显示由 RGBGame 提供的时间,这里使用
Connections 对 象 来 连 接 目 标 对 象 和 信 号 处 理 器 , 指 定 target 为 RGBGame , 在
onCurrentTime 信号处理器中改变 timeLabel 的文本和颜色,颜色用到了 RGBGame 的
timeColor 属性,该属性的读取函数是 timeColor 。
界面中间是一个 Rectangle 对象,id 是 colorRect 。这里使用 Connections 对象,指定
target 为 rgbGame ,在 onColorChanged 信号处理器中改变 colorRect 的颜色。
界面底部是几个按钮,使用锚布局把它们排成一行。 start 按钮的 onClicked 信号处理
器调用 rgbGame 的 start() 槽,启动颜色生成器。 stop 按钮的 onClicked 信号处理器调用
rgbGame 的 stop() 槽,停止颜色生成器。而 colorAlgorithm 按钮则每点击一次就切换一个
颜色生成算法,同时调用 changeAlgorithm() 函数,根据算法改变按钮上的文字。 quit 按钮
点击时退出应用。
 */
