import QtQuick 2.0

Rectangle {
    width: 100
    height: 36
    color: "lightblue"

    Text {
        id:textTest
        states: [
            State {
                name: "redStates"
                PropertyChanges {
                    target: textTest
                    color:"red"
                }
            },//注意这里的逗号
            State {
                name: "blueStates"
                PropertyChanges {
                    target: textTest
                    color:"black"
                }
            }
        ]
        text: qsTr("Hello World")
        anchors.centerIn: parent
        state: "redStates"
    }

    MouseArea {
        anchors.fill: parent
        onReleased: {
            if ("blueStates" == textTest.state)
            {
                textTest.state = "redStates"
            }
            else if ("redStates" == textTest.state)
            {
                textTest.state = "blueStates"
            }
        }
    }
}
