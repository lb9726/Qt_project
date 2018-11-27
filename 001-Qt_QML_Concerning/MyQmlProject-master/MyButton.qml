import QtQuick 2.0
import QtQuick.Controls 1.4
/************************************************
  Date：2017年4月3日
  Author：DreanLife
  About：写一个自己定义的按钮
  **********************************************/
Rectangle
{
    property string nomal_Image: ""
    property string hover_Image: ""
    property string press_Image: ""
//    property string currentImage: ""
    id:root_Button
    width: 50
    height: 30
    color: "transparent"
    state: "nomal"
    Image
    {
        id: button_Background
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
        source: nomal_Image
    }
    Text
    {
        id: button_Text
        anchors.centerIn: parent
        text: qsTr("text")
    }
    MouseArea
    {
        id:button_Mousearea
        anchors.fill: parent
        hoverEnabled: true
        onEntered: root_Button.state="hover"
        onExited: root_Button.state="nomal"
        onPressed:
        {
            root_Button.state="press"
        }
    }
    states:
        [
        State {
            name: "nomal"
            PropertyChanges {
                target:button_Background
                source: nomal_Image

            }
        },
        State {
            name: "hover"
            PropertyChanges {
                target: button_Background
                source: hover_Image
            }
        },
        State {
            name: "press"
            PropertyChanges {
                target: button_Background
                source: press_Image
            }
        }
    ]
    transitions:
        [
        Transition {
            from: "nomal"
            to: "hover"
            PropertyAnimation
            {
                duration: 100
            }

        },
        Transition {
            from: "hover"
            to: "press"
            PropertyAnimation
            {
                duration: 100
            }

        },
        Transition {
            from: "press"
            to: "nomal"
            PropertyAnimation
            {
                duration: 100
            }

        }
    ]
}
