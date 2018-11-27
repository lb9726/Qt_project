import QtQuick 2.3

Rectangle {
    property alias mouseArea: mouseArea

    width: 360
    height: 360

    MouseArea {
        id: mouseArea
        anchors.fill: parent
    }
/*
    Text {
        anchors.centerIn: parent
        text: "Hello World"
    }
    */
    Button {  // 复用Button组件   可见，由于已经编写好了Button.qml文件，此处就可以像使用QML基本元素一样直接使用这个组件。
        x: 25
        y: 25
    }
}
