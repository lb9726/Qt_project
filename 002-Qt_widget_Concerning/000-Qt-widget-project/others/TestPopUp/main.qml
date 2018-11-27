import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0
import QtQuick.Window 2.2
ApplicationWindow {
    id:root
    visible: true
    width: 800
    height: 600
    title: qsTr("测试程序")
    Rectangle
    {
        width: 100
        height: 30
        anchors.right: parent.right
        anchors.top: parent.top
        color: "#FF0000"

        MouseArea
        {
            anchors.fill: parent
            onClicked: {
                if(root.width != Screen.width)
                {
                    showFullScreen()
                    test.text = "ShowNurmal"
                }
                else
                {
                    showNormal()
                    test.text = "ShowFullScreen"
                }
            }
        }
        Text {
            id: test
            anchors.centerIn: parent
            text: qsTr("ShowFullScreen")
        }
    }

    Rectangle
    {
        width: 100
        height: 30
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: 50
        color: "#FFFF00"
        radius: 5
        MouseArea
        {
            anchors.fill: parent
            onClicked: {
                if(textpopup.text == 'Popup')
                {
                    myPopup.open()
                    textpopup.text = "Popup close"

                }
                else
                {
                    myPopup.close()
                    textpopup.text = "Popup"
                }
            }
        }
        Text {
            id: textpopup
            anchors.centerIn: parent
            text: qsTr("Popup")
        }
    }

    Popup
    {
        id:myPopup
        width: 400
        height: 300
        x:(root.width-width)/2
        y:(root.height-height)/2
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

        Rectangle
        {
            anchors.fill: parent
            radius: 20
            Text {
                id: mytext
                font.pixelSize: 24
                text: qsTr("Popup 内容显示模块")
            }
        }
    }
}
