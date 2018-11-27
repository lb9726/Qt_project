import QtQuick 1.1
//import ArrowElement 1.0
Item {
    id:id_rootWindow
    width: windowsHeight
    height: windowsWidth

    property int windowsWidth: 480
    property int windowsHeight: 272
    property int rotateYOffset: (windowsHeight- windowsWidth)/2
//    property string rootpath: "file:///C:/Users/lishaoxiang.SHBST/Desktop/theme/sodimas/"
    property string rootpath: "file:///home/libo/Desktop/Repository/Qt_project/BST_Codes/SodimasDisplay/theme/sodimas/"
    property bool inOperational: false   ///
    property bool isShowLogo: false ///false: Floor,    true : Function Logo
    property bool isFCT_AFF: false ///false: Floor+Arrow , true: email
    property int transformedRot: 0

    Row{
        //visible: false
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
                //x:257
                //y:15
                //width:200
                //height:182
                visible: isShowLogo
                funcImage: rootpath+"function/T41.png"
            }

            Floor{
                id:id_floor
                objectName:"floorObj"
                //x:258
                //y:26
                //width:198
                //height:165
                visible: !isShowLogo
            }

            RealtimeText{
                id:id_realtimetext
                objectName:"realtimetextObj"
                //x:29
                //y:20
                //width:100
                //height:40
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
        z:10
        x:124
        y:15
        width:232
        height: 80
        visible: false
    }
    ProgressBar{
        id:id_progressBar
        objectName: "progressbarObj"
        x:24
        y:210
        width:232
        height: 24
        visible: false
    }
    Desktop{
        id:id_desktop
        objectName: "desktopObj"
        z:-10
    }

    Connections{
        target: GlobalSetting
        onSigInfoProgress:{
            id_progressBar.value = sPercent;
            id_mesageWindow.information = sInformation;
        }
        onSigProgress:{
            id_progressBar.value = percent;
        }
    }

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

    Component.onCompleted: {

    }
}
