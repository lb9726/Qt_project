import QtQuick 2.0

Item {
    id:showarea
    width: 184; height: 162
    clip: true
    Flickable {
        width: showarea.width; height: showarea.height
        contentWidth: image.width; contentHeight: image.height

        Image { id: image; source: "images/ArrowUp.png" }
        SequentialAnimation on y {
            id: gundong
            running: true
            loops: Animation.Infinite
            ParallelAnimation {
                NumberAnimation {
                    from: 0; to: -image.height; duration: 1200; easing.type: Easing.Linear
                }
            }
        }
    }
    Flickable {
        width: showarea.width; height: showarea.height
        contentWidth: image2.width; contentHeight: image2.height

        Image { id: image2; source: "images/ArrowUp.png" }
        SequentialAnimation on y {
            id: gundong2
            running: true
            loops: Animation.Infinite
            ParallelAnimation {
                NumberAnimation {
                    from: image2.height; to: 0; duration: 1200; easing.type: Easing.Linear
                }
            }
        }
    }
}

