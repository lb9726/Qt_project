import QtQuick 2.0

// 主画面坐标为： x:18 ;  y: 451
Item {
    property alias floornum: floornum
    id: floor
    width: 291; height: 177
    Text {
        id: floornum
        font.bold: true
        font.pixelSize: 100
        color: "white"
        text: "9"
        anchors.centerIn: floor
    }
}

