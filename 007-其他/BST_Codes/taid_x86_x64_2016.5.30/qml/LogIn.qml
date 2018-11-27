import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.2

Item{
    //anchors.fill: parent
    Column{
        id:login_area
        anchors.centerIn: parent
        spacing: windowHeight/15

        Rectangle {
            width: windowWidth*3/4 > 1000 ? 1000:windowWidth*3/4
            height: windowHeight*3/4 > 600 ? 600:windowHeight*3/4
            color: viceColor
            clip:true

            Column{
                width: parent.width
                height: parent.height
                spacing: parent.height/8

                Text{
                    width: parent.width
                    height: parent.height/7
                    text:"用户登录"
                    color: "white"
                    font.pixelSize: height/2
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    //anchors.horizontalCenter: parent.horizontalCenter
                }
                Row{
                    width: parent.width*3/4
                    height: parent.height/7
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 20
                    Item{

                        width: parent.width*1/4
                        height: parent.height
                        Text{
                            width: parent.width
                            height: parent.height
                            text:"用户名";
                            color: "white";
                            font.pixelSize: height/2
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                        }

                    }

                    Rectangle{

                        width: parent.width*3/4
                        height: parent.height

                        TextInput{
                            id:usrName
                            font.pixelSize: height
                            anchors.fill: parent
                            color: "black";
                            maximumLength: 16;
                        }
                    }
                }
                Row{
                    width: parent.width*3/4
                    height: parent.height/7
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 20
                    Item{

                        width: parent.width*1/4
                        height: parent.height
                        Text{
                            width: parent.width
                            height: parent.height
                            text:"密码"
                            color: "white"
                            font.pixelSize: height/2
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                    }
                    Rectangle{
                        //anchors.horizontalCenter: parent.horizontalCenter
                        width: parent.width*3/4
                        height: parent.height
                        TextInput{
                            id:passwd
                            font.pixelSize: height
                            anchors.fill: parent
                            color: "black"
                            maximumLength: 16
                        }
                    }
                }
                Item{
                    width: parent.width*3/4
                    height: parent.height/8
                    anchors.right: parent.right
                    /*
                    Row{
                        anchors.fill: parent

                        Item{
                            width:parent.width/2
                            height: parent.height
                            clip:true
                            //color: "red"

                            CheckBox{
                                anchors.centerIn: parent
                                text:"自动登录"
                                width:parent.width/2
                                height:parent.height/2
                                style: CheckBoxStyle {
                                    indicator: Rectangle {
                                        implicitWidth: control.height
                                        implicitHeight: control.height
                                        border.color: "white"
                                        border.width: 1
                                        color:"transparent"
                                        Rectangle {
                                            anchors.fill: parent
                                            visible: control.checked
                                            color: "white"
                                            radius: control.height/2
                                            anchors.margins: 5
                                        }
                                    }
                                    label:Text{
                                        color: "white"
                                        text:control.text
                                        font.pixelSize: control.height*3/4
                                    }
                                    spacing :6

                                }
                            }
                        }

                        Item{
                            width:parent.width/2
                            height: parent.height
                            clip:true
                            CheckBox{
                                anchors.centerIn: parent
                                text:"记住密码"
                                width:parent.width/2
                                height:parent.height/2
                                style: CheckBoxStyle {
                                    indicator: Rectangle {
                                        implicitWidth: control.height
                                        implicitHeight: control.height
                                        border.color: "white"
                                        border.width: 1
                                        color:"transparent"
                                        Rectangle {
                                            anchors.fill: parent
                                            visible: control.checked
                                            color: "white"
                                            radius: control.height/2
                                            anchors.margins: 5
                                        }
                                    }
                                    label:Text{
                                        color: "white"
                                        text:control.text
                                        font.pixelSize: control.height*3/4
                                    }
                                    spacing :6
                                }
                            }
                        }
                    }
                    */
                }
            }
        }

        Rectangle {

            color: viceColor
            width: windowWidth/4
            height: windowHeight/10
            anchors.horizontalCenter: parent.horizontalCenter
            Text{
                text:"登录"
                anchors.centerIn: parent
                color: "white"
                font.pixelSize: parent.height/2
            }
            MouseArea{
                anchors.fill: parent
                onClicked:uiLogicalEngine(2,0)
                onPressed:parent.opacity = themeOpacity;
                onReleased: parent.opacity = 1;
            }
        }
    }

    Button {
        width:windowWidth/20 > 50 ? 50: windowWidth/20
        height:windowWidth/20 > 50 ? 50: windowWidth/20

        anchors{
            left:parent.left
            top:parent.top
            margins: windowWidth > windowHeight  ? windowHeight/20 : windowWidth/20
        }

        style: ButtonStyle {
            background: Image {
                source: "qrc:/images/back.png"
            }
        }
        opacity: pressed ? themeOpacity : 1
        onClicked: uiLogicalEngine(2,1)
    }

    Text{

        width: windowWidth/10 > 50 ? 50: windowWidth/10
        height: windowWidth/20 > 50 ? 50: windowWidth/20
        text:"<u>忘记密码</u>"
        color: "white"
        font.pixelSize: height/2
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        anchors{
            right:parent.right
            bottom:parent.bottom
            margins: windowWidth > windowHeight  ? windowHeight/20 : windowWidth/20
        }

        MouseArea{
            anchors.fill: parent
            onClicked: {}
            onPressed:parent.opacity = themeOpacity
            onReleased: parent.opacity = 1
        }

    }
}
