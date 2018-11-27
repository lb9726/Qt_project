import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
Window {
    id:root
    visible: true
    width: 960*0.7
    height: 1280*0.7
    title: qsTr("QML程序演示")
    color: "#777777"
    flags: Qt.Window | Qt.FramelessWindowHint
    Text
    {
        id:title
        anchors.top:parent.top
        anchors.topMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 5
        font.pointSize: 12
        font.family: "微软雅黑"
        color: "#ffffff"
        text:qsTr("QML程序演示")
    }
    Image {
        id: background
        width: 961*0.6
        height: 1280*0.6
        anchors.centerIn: parent
        source: "qrc:/Images/honey.jpg"
    }

    MyButton
    {
        id: myButton
        width: 30
        height: 30
        y:30
        anchors.right: parent.right
        nomal_Image: "qrc:/Images/button/1.png"
        hover_Image: "qrc:/Images/button/2.png"
        press_Image: "qrc:/Images/button/3.png"
        state: "nomal"

    }

    /***********************************
      Date：2017年4月1日
      Author：DreamLife
      About：鼠标拖拽事件，还没有实现
      Reference:http://blog.csdn.net/xuwq2015/article/details/46681193
      **********************************/
    property int mainWindowX //用来存储主窗口x坐标
    property int mainWindowY //存储窗口y坐标
    property int xMouse //存储鼠标x坐标
    property int yMouse //存储鼠标y坐标
    MouseArea
    {
        id:move
        height: 30
        width: parent.width
        anchors.top:parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        acceptedButtons: Qt.LeftButton //只处理鼠标左键
        //        onPressed:
        //        { //接收鼠标按下事件
        //            xMouse = mouseX
        //            yMouse = mouseY
        //            mainWindowX = root.x
        //            mainWindowY = root.y
        //        }

        property point clickPos: "0,0"
        onPressed:
        { //接收鼠标按下事件
            clickPos  = Qt.point(mouse.x,mouse.y)
        }
        onPositionChanged:
        { //鼠标按下后改变位置
            //            root.x = mainWindowX + (mouseX - xMouse)
            //            root.y = mainWindowX + (mouseY - yMouse)

            //鼠标偏移量
            var delta = Qt.point(mouse.x-clickPos.x, mouse.y-clickPos.y)

            //如果root继承自QWidget,用setPos
            root.setX(root.x+delta.x)
            root.setY(root.y+delta.y)
        }

    }

    Rectangle
    {
        id:close_Btn
        width: 50
        height: 30
        color: "transparent"
        state: "nomal"
        anchors.top: parent.top
        anchors.right: parent.right
        Text {
            id: close_Text
            anchors.centerIn: parent
            text: qsTr("X")
        }
        MouseArea
        {
            id:close_MouseArea
            hoverEnabled: true
            anchors.fill: parent
            onClicked:
            {
                close_Btn.state="press"
                Qt.quit()
            }
            onEntered: close_Btn.state="hover"
            onExited: close_Btn.state="nomal"
        }
        states:
            [
            State
            {
                name: "nomal"
                PropertyChanges
                {
                    target: close_Btn
                    color:"#777777"
                }
            },
            State
            {
                name: "hover"
                PropertyChanges
                {
                    target: close_Btn
                    color:"red"
                }
            },
            State
            {
                name: "press"
                PropertyChanges
                {
                    target: close_Btn
                    color:"blue"
                }
            }
        ]
        transitions:
            [
            Transition
            {
                from: "nomal"
                to: "hover"

                ColorAnimation
                {
                    target: close_Btn
                    duration: 100
                }
            },
            Transition
            {
                from: "hover"
                to: "press"

                ColorAnimation
                {
                    target: close_Btn
                    duration: 100
                }
            },
            Transition
            {
                from: "press"
                to: "nomal"

                ColorAnimation
                {
                    target: close_Btn
                    duration: 100
                }
            }
        ]
    }
}
