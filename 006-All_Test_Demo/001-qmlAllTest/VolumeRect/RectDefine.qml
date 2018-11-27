import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4

Item {
    width: 40
    height: 40
    signal rectClick(int index)
    property  variant colorList: [5]
    RowLayout {
        spacing: 5
        Rectangle {
            id: rect1
            width: 5
            height: 5
            color: "grey"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    rectClick(1)
                }
            }
        }
        Rectangle {
            id: rect2
            width: 5
            height: 10
            color: "grey"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    rectClick(2)
                }
            }
        }
        Rectangle {
            id: rect3
            width: 5
            height: 15
            color: "grey"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    rectClick(3)
                }
            }
        }
        Rectangle {
            id: rect4
            width: 5
            height: 20
            color: "grey"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    rectClick(4)
                }
            }
        }
        Rectangle {
            id: rect5
            width: 5
            height: 25
            color: "grey"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    rectClick(5)
                }
            }
        }
    }
    function setColor(pindex)
    {
        if (pindex === 1)
        {
            rect1.opacity = 1
            rect2.opacity = 0.5
            rect3.opacity = 0.5
            rect4.opacity = 0.5
            rect5.opacity = 0.5
        }
        else if (pindex === 2)
        {
            rect1.opacity = 1
            rect2.opacity = 1
            rect3.opacity = 0.5
            rect4.opacity = 0.5
            rect5.opacity = 0.5
        }
        else if (pindex === 3)
        {
            rect1.opacity = 1
            rect2.opacity = 1
            rect3.opacity = 1
            rect4.opacity = 0.5
            rect5.opacity = 0.5
        }
        else if (pindex === 4)
        {
            rect1.opacity = 1
            rect2.opacity = 1
            rect3.opacity = 1
            rect4.opacity = 1
            rect5.opacity = 0.5
        }
        else if (pindex === 5)
        {
            rect1.opacity = 1
            rect2.opacity = 1
            rect3.opacity = 1
            rect4.opacity = 1
            rect5.opacity = 1
        }
    }
    onRectClick: {
        setColor(index)
    }
    Component.onCompleted: {

    }
}
