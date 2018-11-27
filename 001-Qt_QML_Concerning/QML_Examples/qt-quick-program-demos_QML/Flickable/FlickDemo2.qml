import QtQuick 2.0

Item {
    width: 184; height: 162
    clip: true
    Flickable {
        width: 184; height: 162
        contentWidth: image.width; contentHeight: image.height

        Image { id: image; source: "images/ArrowUp.png" }
        SequentialAnimation on y {
            id: gundong
            running: true
//            loops: Animation.Infinite
            ParallelAnimation {
                NumberAnimation {
                    from: image.height; to: 0; duration: 1200; easing.type: Easing.Linear
                }
            }
        }
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            gundong.running = true       // 开启水平方向（矩形往返) 动画
            //            imgAnim.running = true        // 开启垂直方向（照片掉落）动画
        }
    }
}

