import QtQuick 2.0

Rectangle {
    width: 80
    height: 80
    color: "lightblue"
    radius: 10
    Text {
        anchors.centerIn: parent
        font.pointSize: 12
        text: "改变行为"
    }

    Behavior on x {                         // (e)
        PropertyAnimation {
            duration: 1000                  // 运动时间为1秒
            easing.type: Easing.InQuart     // 加速运动
        }
    }

    Behavior on y {                         // 应用到y轴方向上的运动
        PropertyAnimation {
            duration: 1000                  // 运动时间为1秒
            easing.type: Easing.InQuart     // 加速运动
        }
    }
}

