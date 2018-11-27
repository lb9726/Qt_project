import QtQuick 2.0
import lb2616.tools.FileUpdate2 1.0
Item {
    objectName: "arrowdownpicture"
    id: showrect
    width: 184; height: 162
    clip: true
    FileUpdate2 {
        id:fup
    }

    Flickable {
        width: showrect.width; height: showrect.height
        contentWidth: image2.width; contentHeight: image2.height

        Image { id: image2; source: "images/ArrowDn.png" }
        SequentialAnimation on y {
            id: arrowdown2
            running: false
            loops: 2/*Animation.Infinite*/
            ParallelAnimation {
                NumberAnimation {
                    from: -image2.height; to: 0; duration: 1200; easing.type: Easing.Linear
                }
            }
        }
    }

    Flickable {
        width: showrect.width; height: showrect.height
        contentWidth: image.width; contentHeight: image.height

        Image { id: image; source: "images/ArrowDn.png" }
        SequentialAnimation on y {
            id: arrowdown
            running: false
            loops: 2/*Animation.Infinite*/
            ParallelAnimation {
                NumberAnimation {
                    from: 0; to: image.height; duration: 1200; easing.type: Easing.Linear
                }
            }
        }
    }

    Connections {
        target: fup
        onSendStartAniDown: {
            arrowdown.running = animstardown;
            arrowdown2.running = animstardown;
        }
    }
}

