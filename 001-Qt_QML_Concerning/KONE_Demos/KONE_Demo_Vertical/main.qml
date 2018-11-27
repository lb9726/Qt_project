import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
ApplicationWindow {
    id:win
    visible: true
    color: "black"
    width: 768; height: 1024
//    contentOrientation: Qt.LandscapeOrientation
//    flags: Qt.PortraitOrientation
//     Rotation {
//        axis {
//            x:1024/2
//            y:768/2
//            z:0
//        }
//        angle: 90
//    }
    Rectangle {
        rotation: 270
        color: "transparent"
        anchors.fill: parent

        ShowTitle {
            x:36; y:24
            width: 330; height: 50
        }

        ArrowAnimation {
            x:40; y:114
            width: 156; height: 174
        }
        FloorNumberShow {
            x:204; y:104
            width: 218; height: 182
        }
        ShowWifi {
            x:40; y:288
            width: 156; height: 78
        }

        DateandTime {
            x:614; y:30
            width: 140; height: 86
        }

        MultiMedia {
            x:0; y:384
            width: 768; height: 579
        }
        Rollingtext {
            x:0; y:962
            width: 768; height: 63
        }
    }
}

