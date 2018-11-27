import QtQuick.Controls.Material 2.0
import QtQuick 2.4
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

ApplicationWindow {
    id: win
    visible: true
    property string windowBgColor: "white"

    width: 800
    height:800
    x:100
    y:100

    MouseArea {
        id:fill_mouse
        anchors.fill: parent
        property point clickPos: "0,0"
        propagateComposedEvents: true
        onClicked: {
//            rect.border.color = "black"
            console.log("onClicked:", mouse.x,mouse.y)
        }
        onPressed: {
            clickPos = Qt.point(mouse.x,mouse.y)
            console.log("onPressed:", mouse.x,mouse.y)
        }

        onWheel: {
            if (wheel.angleDelta.y > 0) {
                rect.width += 5;
                rect.x -= 2

                rect.height += 5;
                rect.y -= 2
            }
            else {
                rect.width -= 5;
                rect.x += 2

                rect.height -= 5;
                rect.y += 2
            }
        }
        onPositionChanged: {
            mouse.accepted = false;
            var deltdata = Qt.point(mouseX-clickPos.x, mouseY-clickPos.y);
            if (win.width >= rect.width || win.height >= rect.height)
            {
                //Move win
                win.x = win.x + deltdata.x
                win.y = win.y + deltdata.y

                console.log("onPositionChanged:up", deltdata.x,deltdata.y)
            }
            else
            {
                //Move rect
                rect.x += deltdata.x
                rect.y += deltdata.y

                console.log("onPositionChanged:down", deltdata.x,deltdata.y)
            }
        }
    }

    Rectangle {
        id: rect
        width: 150
        height:150
        anchors.centerIn: parent
        x: parent.x + 60
        y: parent.y + 60
        border.color: "white"
        radius:50

        Image {
            id: img
            anchors.fill: parent
            fillMode: Image.PreserveAspectFit
            source: "images/Lighthouse.jpg"

            MouseArea {
                id: big_mouse
                anchors.fill: parent
                property point clickPos: "0,0"
                onClicked: {
                    mouse.accepted = false
//                    rect.border.color = "red"
                }

                onPressed: {
                    clickPos = Qt.point(mouse.x,mouse.y)
                    console.log("onPressed:", mouse.x,mouse.y)
                }
                onPositionChanged: {
                    mouse.accepted = false;
                    var deltdata = Qt.point(mouseX-clickPos.x, mouseY-clickPos.y);
                    if (win.width >= rect.width || win.height >= rect.height)
                    {
                        //Move win
                        win.x = win.x + deltdata.x
                        win.y = win.y + deltdata.y

                        console.log("In-onPositionChanged:up", deltdata.x,deltdata.y)
                    }
                    else
                    {
                        //Move rect
                        rect.x = rect.x + deltdata.x
                        rect.y = rect.y + deltdata.y

                        console.log("In-onPositionChanged:down", rect.x,rect.y)
                    }
                }
            }
        }

        Image {
            id: img_left
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
//            source: "images/white.jpg"

            MouseArea {
                id: small_mouse
                anchors.fill: parent
                onClicked: {
                    mouse.accepted = false
//                    rect.border.color = "blue"
                }
            }
        }
    }
}
