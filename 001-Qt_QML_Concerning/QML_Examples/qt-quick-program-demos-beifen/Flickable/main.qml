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
}


