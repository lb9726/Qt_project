import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.2

ApplicationWindow {
    id:id_mediaScreenMaintainer
    visible: true
    property int windowsHeight: 480
    property int windowsWidth: 640
    maximumHeight:windowsHeight
    maximumWidth:windowsWidth
    minimumHeight:windowsHeight
    minimumWidth:windowsWidth
    title: qsTr("MediaScreenMaintainer") + "  V1.6.0.  For SHBST MediaScreen Only."
    property string apkPath: ""

    background: Image{
        anchors.fill: parent
        opacity: 0.8
        source: "qrc:///background.png"
    }

    ColumnLayout{
        width: id_mediaScreenMaintainer.width*3/4
        height: id_mediaScreenMaintainer.height*3/4
        anchors.centerIn: parent
        Row{
            width: parent.width
            height: parent.height/6
            //spacing: 10
            Item{
                width: parent.width/4
                height: parent.height
                Label{
                    anchors.centerIn: parent
                    text: "Select Apk File"
                    color: "white"
                }
            }
            Item{
                width: parent.width/2
                height: parent.height
                clip:true
                Label{
                    width: parent.width
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Text.AlignVCenter
                    //anchors.centerIn: parent
                    text: apkPath
                    elide: Text.ElideMiddle
                }
            }
            Item{
                width: parent.width/4
                height: parent.height
                Button{
                    anchors.centerIn: parent
                    text:"Pick Apk"
                    onClicked: {
                        id_pickApk.open();
                    }
                }
            }

        }
        Row{
            width: parent.width
            height: parent.height/6
            //spacing: 10
            Item{
                width: parent.width/4
                height: parent.height
                Label{
                    anchors.centerIn: parent
                    text: "Make Update Disk"
                    color: "white"
                }
            }
            Item{
                width: parent.width/2
                height: parent.height
                ComboBox{
                    id:id_currentUsbDriver
                    anchors.centerIn: parent
                    textRole: "key"
                    enabled: id_driverModel.count > 0 && apkPath != ""
                    model: id_driverModel
                }
            }

            Item{
                width: parent.width/4
                height: parent.height
                Button{
                    anchors.centerIn: parent
                    text:"Make Disk"
                    enabled: id_driverModel.count > 0 && id_currentUsbDriver.currentText !=""
                    onClicked: {
                        if(id_currentUsbDriver.currentText != ""){
                            countDownTimes = 0;
                            makeDisk();
                        }else{

                        }
                    }
                }
            }

        }
    }

//    Text{
//        text: "For SHBST Production MediaScreen Only."
//        color: "red"
//        //font.bold: true
//        anchors.bottom: parent.bottom
//        anchors.horizontalCenter: parent.horizontalCenter
//    }

    ListModel {
        id:id_driverModel
    }

    property alias informationON: id_informationWindow.visible
    property string informationContent: ""

    Popup {
        id: id_informationWindow
        x:(id_mediaScreenMaintainer.width - width)/2
        y:(id_mediaScreenMaintainer.height - height)/2
        width: 600
        height: 300
        modal: true
        focus: true
        closePolicy: Popup.NoAutoClose
        Item{
            width: 600
            height: 300
            anchors.centerIn: parent
            Text {
                id: id_informationContent
                width: 500
                height: 200
                anchors.top: parent.top
                anchors.topMargin: 50
                anchors.horizontalCenter: parent.horizontalCenter
                text: informationContent
                wrapMode:Text.WrapAnywhere
            }
            RowLayout{
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
                height: 50
                spacing: 50
                Button{
                    text:"OK"
                    onClicked:{
                        id_informationWindow.close();
                    }
                }
            }
        }
    }

    property alias progressON: id_progressWindow.visible
    property string progressContent: ""
    property string otherInfoContent: ""
    property int fileProgress: 0
    property bool showProgress: false
    property bool showFinishedButton: false
    Popup {
        id: id_progressWindow
        x:(id_mediaScreenMaintainer.width - width)/2
        y:(id_mediaScreenMaintainer.height - height)/2
        width: 600
        height: 300
        modal: true
        closePolicy: Popup.NoAutoClose
        focus: true
        ColumnLayout{
            spacing: 50
            width: 400
            height: 200
            anchors.centerIn: parent
            ProgressBar{
                id:id_copyProgressBar
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter
                from: 0
                to:100
                value: fileProgress
                visible: showProgress
            }
            Text{
                width: 100
                //width: parent.width
                horizontalAlignment: Text.AlignHCenter
                anchors.horizontalCenter: parent.horizontalCenter
                text: id_copyProgressBar.value +"%"
                visible: showProgress
            }

            Text {
                id: id_progressContent
                anchors.horizontalCenter: parent.horizontalCenter
                text: progressContent+"\n\n"+otherInfoContent
                horizontalAlignment: Text.AlignHCenter
                wrapMode:Text.WrapAnywhere
            }
            Button{
                text:"Click to Finish"
                visible: showFinishedButton
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked:{
                    makeDiskFinished();
                }
            }
        }
    }

    function makeDisk(){
        if(apkPath === ""){
            return;
        }
        ConfigureSerialer.setApkPath(apkPath);//清除上次的配置参数
        progressContent = "Formating USB Driver"
        progressON = true;
        UsbHelper.formatUdisk(id_currentUsbDriver.currentText);
        id_copyTimer.start();
    }

    property int countDownTimes:0
    Timer{
        id:id_copyTimer
        interval:1000
        repeat: true
        onTriggered: {
            countDownTimes ++;
            if(countDownTimes>=30){
                otherInfoContent = ""
                id_copyTimer.stop();
                countDownTimes = 0;
                progressContent = "Copying files"
                showProgress = true;
                ConfigureSerialer.orderFile(id_currentUsbDriver.currentText);
                makeDiskStage2();
            }else{
                otherInfoContent = "( " + (30 -countDownTimes)+" )";
            }
        }
    }

    function makeDiskStage2(){
        showProgress = false;
        progressContent = "Generate configure file";
        ConfigureSerialer.createXml(id_currentUsbDriver.currentText);
        progressContent = "Make Disk Succeed";
        showFinishedButton = true;
    }

    function makeDiskFinished(){
        progressContent = ""
        progressON = false;
        showProgress = false;
        showFinishedButton = false;
        enterFlag = false;
    }

    property bool enterFlag: false

    Connections {
        target: ConfigureSerialer
        onCopyFileProgress:{
            fileProgress = pro;
            if(pro == 100&&enterFlag==false){
                enterFlag = true;
                makeDiskStage2();
            }
        }
    }

    Connections{
        target:UsbHelper
        onSigDeviceAdd:{
            id_driverModel.append({"key": dir});
        }
        onSigDeviceRemove:{
            for(var i=0;i<id_driverModel.count;i++){
                console.log(id_driverModel.get(i).key);
                if(id_driverModel.get(i).key === dir){
                    id_driverModel.remove(i)
                }
            }
        }
    }


    FileDialog {
        id: id_pickApk
        title: "Please choose a video file"
        folder: shortcuts.home
        selectMultiple:false
        nameFilters: [ "apk files (*.apk)", "All files (*)" ]
        onAccepted: {
            apkPath = shiftAbsolutePath(id_pickApk.fileUrl.toString());
        }
    }

    function shiftAbsolutePath(path){
        var pre = path.lastIndexOf("file:///");
        if(pre >= 0){
            return path.substring(8);
        }else{
            return "";
        }
    }

    Component.onCompleted: {
        UsbHelper.scanHD();
    }

}
