import QtQuick 2.0

Rectangle {
    width: 60
    height: 30
    clip: true
    color: "green"
    signal rectClick(int index)
    property  variant opacityList: [5]
    property string imgSrc: ""
    Image {
        id: closeVolume
        width: 20
        height: 20
        source: imgSrc
        anchors.left: rect1.left
        anchors.bottom: rect1.bottom
        MouseArea {
            anchors.fill: parent
            onDoubleClicked: {
                imgSrc = "";
                recoveryOpacity();
                rect1.enabled = true
                rect2.enabled = true
                rect3.enabled = true
                rect4.enabled = true
                rect5.enabled = true
            }
        }
    }

    Rectangle {
        id: rect1
        width: 5
        height: 5
        color: "grey"
        anchors.left: parent.left
        anchors.bottom: rect5.bottom
        anchors.leftMargin: 5
        MouseArea {
            anchors.fill: parent
            onClicked: {
                rectClick(1)
            }
            onDoubleClicked: {
                imgSrc = "qrc:/images/cha.png"
                recordLastOpacity();
                rect1.opacity = 0.5
                rect2.opacity = 0.5
                rect3.opacity = 0.5
                rect4.opacity = 0.5
                rect5.opacity = 0.5
                rect1.enabled = false
                rect2.enabled = false
                rect3.enabled = false
                rect4.enabled = false
                rect5.enabled = false
            }
        }
    }
    Rectangle {
        id: rect2
        width: 5
        height: 10
        color: "grey"
        anchors.left: rect1.left
        anchors.bottom: rect5.bottom
        anchors.leftMargin: 10
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
        anchors.left: rect2.left
        anchors.bottom: rect5.bottom
        anchors.leftMargin: 10
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
        anchors.left: rect3.left
        anchors.bottom: rect5.bottom
        anchors.leftMargin: 10
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
        anchors.left: rect4.left
//        anchors.bottom: rect1.bottom
        anchors.top: parent.top
        anchors.topMargin: (parent.height - rect5.height) / 2.0
        anchors.leftMargin: 10
        MouseArea {
            anchors.fill: parent
            onClicked: {
                rectClick(5)
            }
        }
    }

    function setColor(pindex)
    {
        console.log("pindex = " + pindex)
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
    function recordLastOpacity()
    {
        opacityList[0] = rect1.opacity
        opacityList[1] = rect2.opacity
        opacityList[2] = rect3.opacity
        opacityList[3] = rect4.opacity
        opacityList[4] = rect5.opacity
    }

    function recoveryOpacity()
    {
        rect1.opacity = opacityList[0]
        rect2.opacity = opacityList[1]
        rect3.opacity = opacityList[2]
        rect4.opacity = opacityList[3]
        rect5.opacity = opacityList[4]
    }

    onRectClick: {
        setColor(index)
    }
}
