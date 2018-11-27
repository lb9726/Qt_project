import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.2

Item {
    id: root
    anchors.fill: parent
    PathAnimation {
        id: welcome_animation
        path: Path {
            startX: 120; startY: 100
            PathAttribute { name: "opacity"; value: 0.6 }
            PathCurve { x: 200; y: 200}
            PathAttribute { name: "opacity"; value: 0.8 }
            PathCurve { x: 120; y: 250}
            PathAttribute { name: "opacity"; value: 0.8 }
            PathCurve { x: 400; y: 50}
            PathAttribute { name: "opacity"; value: 0 }
        }
        target : family_logo
        duration: 5000
    }

    Image {
        id: family_logo
        width: parent.width/8*1.6
        height: parent.height/8
        source: "qrc: /images/iFamily.png"
    }

    Component.onCompleted: {
        welcome_animation.start()
    }
}
