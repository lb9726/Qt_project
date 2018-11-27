import QtQuick 2.0
//import lb2616.tools.ShowFloor 1.0
Item {
    objectName: "Arrowdownpicture"
    id: showrect
    width: 156; height: 191
    clip: true

    Flickable {
        width: showrect.width; height: showrect.height
        contentWidth: image2.width; contentHeight: image2.height

        Image { id: image2; source: "images/kone_arrow_down.png" }
        SequentialAnimation on y {
            id: arrowdown2
            running: true
            loops: /*2*/Animation.Infinite
            ParallelAnimation {
                NumberAnimation {
                    from: -image2.height-20; to: -20; duration: 1200; easing.type: Easing.Linear
                }
            }
        }
    }

    Flickable {
        width: showrect.width; height: showrect.height
        contentWidth: image.width; contentHeight: image.height

        Image { id: image; source: "images/kone_arrow_down.png" }
        SequentialAnimation on y {
            id: arrowdown
            running: true
            loops: /*2*/Animation.Infinite
            ParallelAnimation {
                NumberAnimation {
                    from: 0; to: image.height; duration: 1200; easing.type: Easing.Linear
                }
            }
        }
    }

//    Connections {
//        target: fup
//        onSendShowPicture: {
//            arrowdown.running = Up;
//            arrowdown2.running = Up;
//            arrowdown.stop();
//            image.visible = false;
//        }
//    }
//    function hidepic()
//    {

//        image2.visible = false
//    }

}

