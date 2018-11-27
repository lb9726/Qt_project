import QtQuick 2.0

Rectangle {
    property int num: 0
    property alias mtttt: rectTest
    id: rectTest
    color: "red"
    MouseArea {
        anchors.fill: parent
        onClicked: {
            changeColor();
        }
    }

    function changeColor()
    {
        if (0 === num)
        {
            rectTest.color = "green";
        }
        else if (1 === num)
        {
            rectTest.color = "black";
        }
        else if (2 === num)
        {
            rectTest.color = "red";
        }
        ++num;
        if (3 === num)
        {
            num = 0;
        }
    }
}
