import QtQuick 2.5
import QtQuick.Window 2.2

Window {
    width: 1024
    height: 768
    visible: true
    Image {
        id: backg
        source: "images/background.png"
    }
    BstLogo {
        x: 294
        y: 39
    }

//    FlickDemo {
//        x:47
//        y:113
//    }
//    FlickDemo2 {
//        x:47
//        y:113
//    }
//    ArrowUpAnimation
//    {
//        x:47
//        y:113
//    }
    ArrowDownAnimation
    {
        x:47
        y:113
    }
    Rollingtext {
        x: 290
        y: 684
    }
    FileUpdate
    {
        width: parent.width
        height: parent.height
        anchors.centerIn: parent
    }
//    Test {

//    }
    DateandTime {
        x: 779
        y: 27
    }
    FloorNumberShow {
        x:18 ; y: 451
    }
    MultiMedia {
        x: 350; y: 128
        width:675
        height: 511
    }
}


