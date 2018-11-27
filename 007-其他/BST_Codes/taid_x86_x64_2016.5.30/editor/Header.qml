import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import "editor.js" as Code

Item{
    id:id_title
    property string  reslutionInfo: ""
    property string  modelInfo: "BVT507"

    signal switchHV(bool hv); //false - horizontal; true - vertical
    signal switchRotation(int ro);//
    signal screenShot();
    signal saveProject();

    property alias m_AutoMode: id_AutoMode.checked

    property int  cWidth: id_title.width/8
    property int  cHeight: id_title.height

    Row{
        anchors.fill: parent
        Item {
            clip: true
            width:cWidth
            height: cHeight
            Button {
                id:id_back
                width: parent.height*3/5
                height: parent.height*3/5
                anchors.centerIn: parent
                opacity: pressed ? themeOpacity : 1
                style: ButtonStyle {
                    background: Image {
                        source: "qrc:/images/back.png"
                    }
                }
                onClicked: {
                    uiLogicalEngine(8,0)
                }
                state:m_AutoMode ? "Auto":"Normal"
                states: [
                    State {
                        name: "Normal"
                        PropertyChanges { target: id_back; enabled: true;opacity:1 }
                    },
                    State {
                        name: "Auto"
                        PropertyChanges { target: id_back; enabled: false;opacity:0.6 }
                    }
                ]
            }
        }
        Item {
            clip: true
            width:cWidth
            height: cHeight
            Text{
                text:"Model : "+ rootWindow.deviceModel
                color: "white"
                anchors.centerIn: parent
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                wrapMode:Text.WrapAnywhere
                font.family:"微软雅黑"
            }
        }
        Item {
            clip: true
            width:cWidth
            height: cHeight
            Text{
                text:"Reslution : "+ reslutionInfo
                font.family:"微软雅黑"
                color: "white"
                anchors.centerIn: parent
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                wrapMode:Text.WrapAnywhere
            }
        }
        Item {
            clip: true
            width:cWidth
            height: cHeight
            Row{
                id:id_layout
                anchors.fill: parent
                spacing: width/6

                Text{
                    font.family:"微软雅黑"
                    text:"Layout:"
                    color: "white"
                    height: parent.height
                    verticalAlignment: Text.AlignVCenter
                    wrapMode:Text.Wrap
                    clip: true
                }

                Switch{
                    id:id_hvSwitch
                    width: parent.width*2/3
                    height: parent.height/3
                    anchors.verticalCenter: parent.verticalCenter
                    onCheckedChanged:{
                        switchHV(checked)
                    }
                }

                state:"Normal" ;//m_AutoMode ? "Auto":"Normal"

                states: [
                    State {
                        name: "Normal"
                        PropertyChanges { target: id_layout; enabled: true;opacity:1 }
                    },
                    State {
                        name: "Auto"
                        PropertyChanges { target: id_layout; enabled: false;opacity:0.6 }
                    }
                ]
            }
        }
        Item {
            clip: true
            width:cWidth
            height: cHeight

            Row{
               anchors.fill: parent
                spacing: width/6
                Text{
                    font.family:"微软雅黑"
                    text:"AutoMode:"
                    color: "white"
                    //width: parent.width/3
                    height: parent.height
                    verticalAlignment: Text.AlignVCenter
                    wrapMode:Text.Wrap
                    clip: true
                }

                Switch{
                    id:id_AutoMode
                    width: parent.width*2/3
                    height: parent.height/3
                    anchors.verticalCenter: parent.verticalCenter
                    opacity: enabled ? 1 : 0.6
                    onCheckedChanged:{
                        id_AutoMode.enabled = false
                        id_enableSwitch.start()
                    }
                }
            }
        }
        Item {
            clip: true
            width:cWidth
            height: cHeight
            Image {
                width:parent.height*3/5
                height: parent.height*3/5
                source: "qrc:/images/send.png"
                anchors.verticalCenter: parent.verticalCenter
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        screenShot();
                    }
                    onPressed: {
                        parent.scale = 0.8
                    }

                    onReleased: {
                        parent.scale = 1
                    }
                }
            }
        }
        Item {
            clip: true
            width:cWidth
            height: cHeight
            Item {
                width:cWidth
                height: cHeight
                Image {
                    id:id_saveproject
                    opacity: enabled ? 1 : 0.6
                    width:parent.height*3/5
                    height: parent.height*3/5
                    source: "qrc:/images/save.png"
                    anchors.verticalCenter: parent.verticalCenter
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            id_saveproject.enabled = false;
                            saveProject();
                            id_savetimer.start()
                        }
                        onPressed: {
                            parent.scale = 0.8
                        }

                        onReleased: {
                            parent.scale = 1
                        }
                    }
                }
            }
        }
        Item {
            clip: true
            width:cWidth
            height: cHeight
            Button {
                id:bt_publish
                width: parent.height*3/5
                height: parent.height*3/5
                anchors.centerIn: parent
                opacity: pressed ? themeOpacity : 1
                style: ButtonStyle {
                    background: Image {
                        source: "qrc:/images/publish.png"
                    }
                }

                onClicked: {
                    uiLogicalEngine(8,1)
                }

                state: m_AutoMode ? "Auto":"Normal"
                states: [
                    State {
                        name: "Normal"
                        PropertyChanges { target: bt_publish; enabled: true;opacity:1 }
                    },
                    State {
                        name: "Auto"
                        PropertyChanges { target: bt_publish; enabled: false;opacity:0.6 }
                    }
                ]
            }
        }
    }

    Timer{
        id:id_enableSwitch
        interval: 3000
        repeat: false
        onTriggered: {
            id_AutoMode.enabled = true ;
        }
    }

    Timer{
        id:id_savetimer
        interval: 2000
        repeat: false
        onTriggered: {
            id_saveproject.enabled = true ;
        }
    }

    function dynamicPublish(){

    }
}
