import QtQuick 2.0

Item {
    anchors.centerIn: parent
    width: 200; height: 240

    Image { id: background; source: "qrc:/images/clock.png"; /*visible: clock.night == false*/ }
    Image { source: "qrc:/images/clock-night.png"; /*visible: clock.night == true*/ }


    Image {
        x: 92.5; y: 22 + 5
        source: "qrc:/images/hour.png"
        transform: Rotation {
            id: hourRotation
            origin.x: 7.5; origin.y: 78 - 5;
//            NumberAnimation on angle {    // 定义角度angle上的动画
//                from: 0
//                to: 360
//                duration: 3600000
//                loops: Animation.Infinite
//            }
        }
    }

    Image {
        x: 93.5; y: 12
        source: "qrc:/images/minute.png"
        transform: Rotation {
            id: minuteRotation
            origin.x: 6.5; origin.y: 88;
//            NumberAnimation on angle {    // 定义角度angle上的动画
//                from: 0
//                to: 360
//                duration: 360000
//                loops: Animation.Infinite
//            }
        }
    }

    Image {
        x: 97.5; y: 15
        source: "qrc:/images/second.png"
        transform: Rotation {
            id: secondRotation
            origin.x: 2.5; origin.y: 85;
//            NumberAnimation on angle {    // 定义角度angle上的动画
//                from: 0
//                to: 360
//                duration: 60000
//                loops: Animation.Infinite
//            }
        }
    }

    Image {
        id: circle_img
        anchors.centerIn: background; source: "qrc:/images/center.png"
//        width: 22
//        height: 22
    }

    Text {
        id: cityLabel
        y: 210; anchors.horizontalCenter: parent.horizontalCenter
        color: "white"
        font.family: "Helvetica"
        font.bold: true; font.pixelSize: 16
        style: Text.Raised; styleColor: "black"
    }
}

