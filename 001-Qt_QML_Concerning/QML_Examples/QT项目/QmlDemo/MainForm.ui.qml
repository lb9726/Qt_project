import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

Rectangle {
    property alias mouseArea: mouseArea
    property alias topRect: topRect  //
    width: 360
    height: 360
    MouseArea {
        id: mouseArea
        anchors.fill: parent
    }
    /* 添加定义两个Rectangle 对象*/
    Rectangle {
        rotation: 0    //旋转45度
        x: 40           //X方向坐标
        y: 60           //Y方向坐标
        width: 100      //矩形宽度
        height: 100
        color: "red"
    }

    Rectangle {
        id :topRect     //id标志符
        opacity: 0.2    //设置透明度
        scale: 1      //缩小为原尺寸的80%
        x: 135
        y: 60
        width: 100
        height: 100
        radius: 8       //绘制圆角矩形
        gradient: Gradient {
            GradientStop { position: 0.0; color: "aqua" }
            GradientStop { position: 1.0; color: "teal" }
        }
        border { width: 1; color: "blue" }   //为矩形添加一个3像素宽的蓝色边框
    }
    Button {
        id: btn
        x:300
        y:300
        MouseArea {
            ColorAnimation {
                from: "white"
                to: "black"
                duration: 200
            }
        }
    }

}
