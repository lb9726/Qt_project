import QtQuick 2.0
// 绿色矩形，源文件 BlueRectangle.qml
Rectangle {
    width: 80
    height: 50
    color: "blue"
    border.color: Qt.lighter(color)
}

