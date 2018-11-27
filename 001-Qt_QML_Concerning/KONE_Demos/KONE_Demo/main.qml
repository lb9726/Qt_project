import QtQuick 2.5
import QtQuick.Window 2.2

Window {
    visible: true
    width: 1024
    height: 768
    color: "black"
    ShowTitle {
        x: 351; y: 15
    }

//    ArrowDownAnimation {
//        x: 98; y: 131
//    }
    TestAnimation {
        x: 98; y: 131
        width: 156; height: 191
    }

//    DateandTime {
//        x: 884; y: 45
//    }
    FloorNumberShow {
        x: 0; y: 363
        width: 351; height: 173
        text: "2"
    }
    TitleAndTime {
        x: 884; y: 15
    }
    MultiMedia {
        x: 350; y: 128
        width:675
        height: 511
    }
    Rollingtext {
        x: 348; y: 644
    }
    ShowWifi {
        x: 80; y: 582
    }
}

