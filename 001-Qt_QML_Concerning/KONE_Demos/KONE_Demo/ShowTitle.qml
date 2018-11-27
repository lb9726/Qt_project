import QtQuick 2.0

Item {
    id: showtitle
    width: 320; height: 51
    Text {
        x: 0; y: 0
        id:  title
        color: "white"
        text: "Shanghai Mart"
        font.family: "KONE Information_v12"
        font.pixelSize: 60
        anchors.top: showtitle.top
    }
}

