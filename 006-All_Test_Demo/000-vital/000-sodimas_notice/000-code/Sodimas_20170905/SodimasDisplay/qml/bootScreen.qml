import QtQuick 1.1

Item{
    id: id_bootscreen
    objectName: "bootscreenUI"

    property int windowsWidth: 480
    property int windowsHeight: 272
    property int rotateYOffset: (windowsHeight- windowsWidth)/2
    property int transformedRot: 0

    width: windowsWidth
    height: windowsHeight

    Column {
        anchors.centerIn: parent
        spacing: 40
        Image {
            id: id_bootlogo
            objectName: "bootlogo"
            width: 200
            height: 35
            anchors.horizontalCenter: parent.horizontalCenter
        }

        ProgressBar {
            id: id_bootprogressbar
            objectName: "bootprogressbar"
            width: 240
            height: 20
        }

        Text {
            id: id_bootversion
            objectName: "bootversion"
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 20
            color: "white"
        }
    }

    Image {
        id: id_bootbackground
        objectName: "bootbackground"
        anchors.fill: parent
        z: -10
    }

    //state:"horizental"
    states: [
        State {
            name: "horizental"
            when: transformedRot == 0 || transformedRot == 180
            PropertyChanges {
                target: id_bootscreen
                y: 0;
                width:windowsWidth;
                height:windowsHeight;
            }
        },
        State {
            name: "vertical"
            when: transformedRot == 90 || transformedRot == 270
            PropertyChanges {
                target: id_bootscreen
                y: rotateYOffset;
                width: windowsHeight;
                height: windowsWidth ;
            }
        }
    ]

    Connections{
        target: GlobalSetting
        onSigInfoProgress:{            
            if(sPercent >=0 && sPercent<=100)
                id_bootprogressbar.value = sPercent
        }
    }

    function rotateUi(angle){
        transformedRot  = angle
    }
}
