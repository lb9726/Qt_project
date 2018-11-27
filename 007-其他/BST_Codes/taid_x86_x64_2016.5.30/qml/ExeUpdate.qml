import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.2
import QtQuick.Dialogs 1.2
Item{
    property string exePath: ""
    Button {
        width:windowWidth/20 > 50 ? 50: windowWidth/20
        height: windowWidth/20 > 50 ? 50: windowWidth/20
        opacity: pressed ? themeOpacity : 1

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

        onClicked: {
            uiLogicalEngine(9,0)
        }
    }

    Button {
        id:bt_publish
        width: windowWidth/20 > 50 ? 50: windowWidth/20
        height: windowWidth/20 > 50 ? 50: windowWidth/20

        opacity: pressed ? themeOpacity : 1

        anchors{
            right:parent.right
            top:parent.top
            margins: windowWidth > windowHeight  ? windowHeight/20 : windowWidth/20
        }

        style: ButtonStyle {
            background: Image {
                source: "qrc:/images/publish.png"
            }

        }

        onClicked: {
            uiLogicalEngine(9,1)
        }
    }

    Item{
        width: parent.width*3/4
        height: parent.height*3/4
        anchors.centerIn: parent
        //anchors.verticalCenterOffset: parent.height/12
        Row{
            anchors.fill: parent
            spacing: 5
            Column{
                width: parent.width*2/5
                height: parent.height
                spacing: height/50

                Item{
                    width: parent.width
                    height: parent.height/10
                    //color: "red"
                    Text{
                        anchors.fill: parent;
                        text:"在线检测";
                        verticalAlignment: Text.AlignVCenter;
                        font.pixelSize: height/2;
                        color: "white";
                    }
                }

                Item{
                    width: parent.width
                    height: parent.height/10
                    Rectangle {
                        color: viceColor
                        width: parent.width*3/4
                        height: parent.height
                        //anchors.horizontalCenter: parent.horizontalCenter
                        Text{
                            text:"检测更新"
                            anchors.centerIn: parent
                            color: "white"
                            font.pixelSize: parent.height/2
                        }
                        MouseArea{
                            anchors.fill: parent
                            //onClicked:sig_online();
                            onPressed:parent.opacity = themeOpacity;
                            onReleased: parent.opacity = 1;
                        }
                    }
                }

                Item{
                    width: parent.width
                    height: parent.height/10
                    //color: "red"
                    Text{
                        anchors.fill: parent
                        text:"程序版本 : "+"V 1.0 "
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: height/3
                        color: "white"
                    }
                }

                Item{
                    id:change_log_1
                    width: parent.width
                    height: parent.height*4/10
                    //color: "green"
                    ListView{
                        anchors.fill: parent
                        model: 6
                        clip: true
                        delegate: Text {
                            height:change_log_1.height/4
                            width:change_log_1.width
                            text: "  " + index+". "+"nihao"
                            verticalAlignment: Text.AlignVCenter
                            font.pixelSize: height/2
                            color: "white"
                        }
                    }
                }

                Item{
                    width: parent.width
                    height: parent.height/5
                    //color: "green"
                    Row{
                        width: parent.width
                        height: parent.height*3/8
                        spacing: width*1/8
                        anchors.centerIn: parent
                        Text{
                            width: parent.width*3/8
                            height: parent.height
                            text:"是否应用升级"
                            font.pixelSize: height/2
                            color: "white"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        Switch {
                            width: parent.width*3/8
                            height: parent.height
                            //checked: true
                            //anchors.centerIn: parent
                            style: SwitchStyle {
                                groove: Rectangle {
                                    implicitWidth: control.width
                                    implicitHeight: control.height
                                    radius: control.height/2
                                    color: control.checked ? "green" : "gray"
                                    Row{
                                        anchors.fill: parent
                                        Text{
                                            width: control.width/2
                                            height: control.height
                                            text:"ON"
                                            font.pixelSize: height/2
                                            color: "white"
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                        Text{
                                            x:control.width/2
                                            width: control.width/2
                                            height: control.height
                                            text:"OFF"
                                            font.pixelSize: height/2
                                            color: "white"
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                    }
                                }
                                handle: Rectangle{
                                    implicitWidth: control.width/2
                                    implicitHeight: control.height
                                    radius: control.height/2
                                }
                            }
                        }
                    }


                }
            }
            Item{
                width: parent.width/5
                height: parent.height
                Rectangle{
                    height: parent.height
                    width: 3
                    anchors.centerIn: parent
                    opacity: 0.8
                    radius: 3
                }

            }
            Column{
                width: parent.width*2/5
                height: parent.height
                spacing: height/50

                Item{
                    width: parent.width
                    height: parent.height/10
                    Text{
                        anchors.fill: parent
                        text:"本地升级"
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: height/2
                        color: "white"
                    }
                }

                Item{
                    width: parent.width
                    height: parent.height/10
                    Rectangle {
                        color: viceColor
                        width: parent.width*3/4
                        height: parent.height
                        //anchors.horizontalCenter: parent.horizontalCenter
                        Text{
                            text:"加载升级文件"
                            anchors.centerIn: parent
                            color: "white"
                            font.pixelSize: parent.height/2
                        }
                        MouseArea{
                            anchors.fill: parent
                            onClicked:{
                                fileDialog.open();
                            }

                            onPressed:parent.opacity = themeOpacity;
                            onReleased: parent.opacity = 1;
                        }
                    }
                }

                Item{
                    width: parent.width
                    height: parent.height/10
                    //color: "red"
                    Text{
                        anchors.fill: parent
                        text:"程序版本 : "+"V 1.0 "
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: height/3
                        color: "white"
                    }
                }

                Item{
                    id:change_log_2
                    width: parent.width
                    height: parent.height*4/10
                    //color: "green"
                    ListView{
                        anchors.fill: parent
                        model: 6
                        clip: true
                        delegate: Text {
                            height:change_log_2.height/4
                            width:change_log_2.width
                            text: "  " + index+". "+"nihao"
                            verticalAlignment: Text.AlignVCenter
                            font.pixelSize: height/2
                            color: "white"
                        }
                    }
                }

                Item{
                    width: parent.width
                    height: parent.height/5
                    //color: "green"
                    Row{
                        width: parent.width
                        height: parent.height*3/8
                        spacing: width*1/8
                        anchors.centerIn: parent
                        Text{
                            width: parent.width*3/8
                            height: parent.height
                            text:"是否应用升级"
                            font.pixelSize: height/2
                            color: "white"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        Switch {
                            width: parent.width*3/8
                            height: parent.height
                            //checked: true
                            //anchors.centerIn: parent
                            style: SwitchStyle {
                                groove: Rectangle {
                                    implicitWidth: control.width
                                    implicitHeight: control.height
                                    radius: control.height/2
                                    color: control.checked ? "green" : "gray"
                                    Row{
                                        anchors.fill: parent
                                        Text{
                                            width: control.width/2
                                            height: control.height
                                            text:"ON"
                                            font.pixelSize: height/2
                                            color: "white"
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                        Text{
                                            x:control.width/2
                                            width: control.width/2
                                            height: control.height
                                            text:"OFF"
                                            font.pixelSize: height/2
                                            color: "white"
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                    }
                                }
                                handle: Rectangle{
                                    implicitWidth: control.width/2
                                    implicitHeight: control.height
                                    radius: control.height/2
                                }
                            }
                        }
                    }


                }
            }
        }
    }

    FileDialog {
        id: fileDialog
        title: "Please choose folder to publish"
        //folder: shortcuts.home
        nameFilters: [ "All files (*)" ]
        onAccepted: {
            exePath  = fileDialog.fileUrl
            //local_uifile.text = exePath.toString().substring(7)
        }
        onRejected: {
            //console.log("Canceled")
        }
    }

    Timer {
        interval: 1000; running: true; repeat: false
        onTriggered: pullResource()
    }

    function pullResource(){

    }

    function pushResource(exportPath){
        if(exePath!=""){
            GUiEditor.pushExeResource(exportPath,exePath);
        }
    }
}
