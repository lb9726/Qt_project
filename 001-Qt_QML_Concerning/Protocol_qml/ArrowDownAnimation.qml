import QtQuick 2.0
// 主画面坐标为： x:47; y:113
Item {
    id: showrect
    width: 184; height: 162
    clip: true
    Flickable {
        width: showrect.width; height: showrect.height
        contentWidth: image2.width; contentHeight: image2.height

        Image { id: image2; source: "images/ArrowDn.png" }
        SequentialAnimation on y {
            id: arrowdown2
            running: true
            loops: Animation.Infinite
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
            running: true
            loops: Animation.Infinite
            ParallelAnimation {
                NumberAnimation {
                    from: 0; to: image.height; duration: 1200; easing.type: Easing.Linear
                }
            }
        }
    }
}
