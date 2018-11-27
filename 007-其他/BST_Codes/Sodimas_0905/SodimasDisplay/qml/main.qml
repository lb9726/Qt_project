import QtQuick 1.1
//import ArrowElement 1.0
Item {
    id:id_rootWindow
    width: windowsHeight
    height: windowsWidth

    property int windowsWidth: 480
    property int windowsHeight: 272
    property int rotateYOffset: (windowsHeight- windowsWidth)/2
    property bool inOperational: true
    property bool isShowLogo: false ///false: Floor,    true : Function Logo
    property bool isFCT_AFF: false ///false: Floor+Arrow , true: text information
    property int transformedRot: 0
    property bool didBootFinished: false

    Row{
        visible: false
        width: 150
        height: 20
        Rectangle{
            width: 50
            height: 20
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    isShowLogo =!isShowLogo
                }
            }
            color: "grey"
        }
        Rectangle{
            width: 50
            height: 20
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    GlobalSetting.qmlSlotTest();
                }
            }
            color: "red"
        }
        Rectangle{
            width: 50
            height: 20
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    isTextScreen = !isTextScreen
                }
            }
            color: "green"
        }
    }

    Row{
        anchors.right: parent.right
        anchors.top: parent.top
        width: 65
        height: 20
        spacing: 5
        Icon{
            id:id_usbLogo
            objectName: "usbLogoObj"
            width: 30
            height: 20
        }
        Icon{
            id:id_wifiLogo
            objectName: "wifiLogoObj"
            width: 30
            height: 20
        }
    }

    Item{
        visible: inOperational
        anchors.fill: parent
        Item{
            anchors.fill: parent
            visible:!isFCT_AFF

            Arrow{
                id:id_arrow
                objectName:"arrowObj"
                //x:26
                //y:32
                //width: 200
                //height: 200
            }

            Function{
                id:id_function
                objectName:"functionObj"
                visible: isShowLogo
            }

            Floor{
                id:id_floor
                objectName:"floorObj"
                visible: !isShowLogo
            }

            RealtimeText{
                id:id_realtimetext
                objectName:"realtimetextObj"
                isAlignCenter:isShowLogo
            }
        }

        FullText{
            id:id_fulltext
            objectName:"fulltextObj"
            width: 300
            height: 100
            anchors.centerIn: parent
            visible:isFCT_AFF
        }
    }

    MeassageBar{
        id:id_mesagebar
        objectName: "mesagebarObj"
        z:20
        x:124
        y:15
        width:232
        height: 80
        visible: didBootFinished && information != ""
    }

    ProgressBar{
        id:id_progressBar
        objectName: "progressbarObj"
        x:24
        y:210
        width:232
        height: 24
        visible: value > 0
    }

    Desktop{
        id:id_desktop
        objectName: "desktopObj"
        z:-10
    }

    Connections{
        target: GlobalSetting
        onSigProgress:{
            if(didBootFinished&&percent >=0 && percent<=100){
                id_progressBar.value = percent
            }
        }
        onSigInfo:{
            if(didBootFinished){
                id_mesagebar.updateMsg(info);
                //id_mesagebar.information = info
            }
        }
    }

    ///BootScreen
    Column{
        id:id_bootScreen
        anchors.centerIn: parent
        spacing: 40
        Image{
            id:id_bootlogo
            objectName: "bootlogo"
            width: 200
            height: 35
            anchors.horizontalCenter: parent.horizontalCenter
        }

        ProgressBar{
            id:id_bootprogressbar
            objectName: "bootprogressbar"
            width: 240
            height: 20
        }

        Text{
            id:id_bootversion
            objectName: "bootversion"
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 20
            color: "white"
        }
        z:10
    }

    Image {
        id:id_bootbackground
        objectName: "bootbackground"
        anchors.fill: parent
        z:9
    }

    Connections{
        target: GlobalSetting
        onSigProgress:{
            if(percent >=0 && percent<=100){
                id_bootprogressbar.value = percent
            }else{
                bootFinished();
            }
        }
    }

    function bootFinished(){
        didBootFinished = true;
        id_bootbackground.visible = false;
        id_bootScreen.visible = false;
    }
    ///Show floor or logo
    function showFloorOrLogo(isLogo){
        isShowLogo = isLogo;
    }

    ///BootScreen
    function rotateUi(angle){
        transformedRot = angle
    }

    states: [
        State {
            name: "horizental"
            when: transformedRot == 0  ||transformedRot == 180
            PropertyChanges {
                target: id_rootWindow
                y:0;
                width:windowsWidth;
                height:windowsHeight;
            }
        },
        State {
            name: "vertical"
            when: transformedRot == 90  ||transformedRot == 270
            PropertyChanges {
                target: id_rootWindow
                y:rotateYOffset;
                width:windowsHeight;
                height:windowsWidth ;
            }
        }
    ]
}
