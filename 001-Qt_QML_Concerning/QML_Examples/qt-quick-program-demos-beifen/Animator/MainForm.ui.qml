import QtQuick 2.3

Rectangle {
    property alias mouseArea: mouseArea

    width: 360
    height: 360

    MouseArea {
        id: mouseArea
        anchors.fill: parent
    }

    AnimatorRect { }    //使用组件
    GundongText {          // 属性名需要大写
        x: 0
        y: parent.height - 30
    }
}
