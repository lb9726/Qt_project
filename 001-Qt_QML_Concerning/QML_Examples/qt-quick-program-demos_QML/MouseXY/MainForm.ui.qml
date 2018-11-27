import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

Item {
    width: 200; height: 200
    property bool textisPressed: false
//    MouseArea {
//        id: msArea
//        anchors.fill: parent
//        onPressed: label.moveTo(mouse.x, mouse.y)
//        drag.target: label
//        drag.axis: Drag.XAndYAxis
//        drag.minimumX: 0
//        drag.maximumX: parent.width - label.width
//    }

    Text {
        id: label
//        function moveTo(newX, newY) {
//            label.x = newX;
//            label.y = newY;
//        }
        MouseArea {
//            id: msArea
            anchors.fill: parent
    //        onPressed: label.moveTo(mouse.x, mouse.y)
            drag.target: label
            drag.axis: Drag.XAndYAxis
            drag.minimumX: 0
            drag.maximumX: rect1.width - label.width
            drag.minimumY: 0
            drag.maximumY: rect1.height - label.height
            onPressed: {
                textisPressed = true;
                console.log("is pressed",textisPressed);
                console.log(rect1.childAt(mouse.x, mouse.y));
            }
            onReleased: {
                textisPressed = false
                console.log("is released",textisPressed);
            }

            onPositionChanged: {
                console.log(mouse.x,mouse.y);
            }
        }
        text: "Move me!"
    }
}
