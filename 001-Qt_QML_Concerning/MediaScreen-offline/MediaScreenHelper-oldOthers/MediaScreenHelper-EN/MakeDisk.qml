import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Page{
    id:id_pageRoot
    property bool resetChecked: false
    property bool confirmedOnce: false
    clip:true
    header: ToolBar {

    }
    onResetCheckedChanged: {
        if(resetChecked){
            id_mediaScreenHelper.resetEnabled = true
        }else{
            id_mediaScreenHelper.resetEnabled = false
        }
    }
    Row{
        //enabled: !modelAppWindow
        anchors.fill: parent
        Item{
            width: parent.width/2
            height: parent.height/2
            anchors.verticalCenter: parent.verticalCenter
            ColumnLayout{
                anchors.centerIn: parent
                spacing: 50
                Label{
                    text: "Reset Screen Configure"
                    //width: 100
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                Switch {
                    id: control
                    //text: qsTr("Switch")
                    anchors.horizontalCenter: parent.horizontalCenter
                    indicator: Rectangle {
                        implicitWidth: 60
                        implicitHeight: 26
                        x: control.leftPadding
                        y: parent.height / 2 - height / 2
                        radius: 13
                        color: control.checked ? "#17a81a" : "#ffffff"
                        border.color: control.checked ? "#17a81a" : "#cccccc"

                        Rectangle {
                            x: control.checked ? parent.width - width : 0
                            Behavior on x {
                                NumberAnimation { duration: 300 }
                            }
                            width: 26
                            height: 26
                            radius: 13
                            color: control.down ? "#cccccc" : "#ffffff"
                            border.color: control.checked ? (control.down ? "#17a81a" : "#21be2b") : "#999999"
                        }
                    }
                    contentItem: Text {
                        text: control.text
                        font: control.font
                        opacity: enabled ? 1.0 : 0.3
                        color: control.checked ? "#ffffff" : "black"
                        enabled: control.checked
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        leftPadding: control.indicator.width + control.spacing
                    }

                    checked: resetChecked

                    MouseArea{
                        anchors.fill: parent
                        //enabled: !resetChecked
                        onClicked: {
                            if(resetChecked){
                                resetChecked = false;
                                confirmedOnce = false;

                            }else{
                                id_resetWarnningBox.open();
                            }
                        }
                    }
                    //anchors.leftMargin: 250
                    anchors.left: parent.left
                }
            }

        }
        Item{
            width: parent.width/2
            height: parent.height/2
            anchors.verticalCenter: parent.verticalCenter
            ColumnLayout{
                anchors.centerIn: parent
                spacing: 50
                Label{
                    text: "Make Update Disk"
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                RowLayout{
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 50
                    ComboBox{
                        id:id_currentUsbDriver
                        width: 10
                        height: 50
                        textRole: "key"
                        enabled: id_driverModel.count > 0
                        model: id_driverModel
                    }

                    Button{
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
    }

    ListModel {
        id:id_driverModel
        //ListElement { key: "First"}
        //ListElement { key: "Second"}
        //ListElement { key: "Third"}
    }

    Popup {
        id: id_resetWarnningBox
        x:(id_pageRoot.width - width)/2
        y:(id_pageRoot.height - height)/2
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
                id: id_resetContent
                width: 500
                height: 200
                anchors.top: parent.top
                anchors.topMargin: 50
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("By <b>Open/ON </b> this <b>Reset switcher</b>, the screen with reset to the factory setting. <i>Multimedia</i> , <i>Scrolling text</i>, <i>Title</i>, <i>Time & Date</i>, <i>System parameters</i> , all those component's resources ,formats and parameters will reset to default")
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
                        //                        if(confirmedOnce == false){
                        //                            confirmedOnce = true;
                        //                            id_resetRepeter.start();
                        //                            id_resetWarnningBox.close();
                        //                        }else{
                        resetChecked = true
                        id_resetWarnningBox.close();
                        //}
                    }
                }
                Button{
                    text:"Cancel"
                    onClicked: id_resetWarnningBox.close();
                }
            }
        }
    }

    Timer{
        id:id_resetRepeter
        interval: 3000
        repeat: false
        onTriggered: {
        }
    }

    property alias informationON: id_informationWindow.visible
    property string informationContent: ""

    Popup {
        id: id_informationWindow
        x:(id_pageRoot.width - width)/2
        y:(id_pageRoot.height - height)/2
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
        x:(id_pageRoot.width - width)/2
        y:(id_pageRoot.height - height)/2
        width: 600
        height: 300
        modal: true
        closePolicy: Popup.NoAutoClose
        focus: true
        //visible: true
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
                //width: 500
                //height: 200
                //anchors.top: parent.top
                //anchors.topMargin: 50
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


    function makeDiskFinished(){
        progressContent = ""
        progressON = false;
        showProgress = false;
        showFinishedButton = false;
        enterFlag = false;
    }

    function makeDisk(){
        //console.log("1")
        if(!multiMediaView.multimediaSwitcher&&
                !parameterView.bConfigureParameter&&
                !resetChecked){
            console.log("2")
            return;
        }
       // console.log("3")
        var ret = multiMediaView.checkMediaSetting();
        if(ret !== "" && !informationON){
            //id_debugger.text = "here2"
            informationContent = ret;
            informationON = true;
            return;
        }

        ret = parameterView.checkParameterSetting();
        if(ret !== "" && !informationON){
            informationContent = ret;
            informationON = true;
            //id_debugger.text = "here3"
            return;
        }

        ConfigureSerialer.clearParameters();//清除上次的配置参数

        if(resetChecked){
            ConfigureSerialer.resetScreen(true);
        }else{
            multiMediaView.updateMediaSetting();
            parameterView.updateParameterSetting();
        }

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
                if(multiMediaView.multimediaSwitcher){
                    progressContent = "Copying files"
                    if(multiMediaView.choseVideo){
                        showProgress = true;
                    }else{
                        showProgress = false;
                    }
                    ConfigureSerialer.orderFile(id_currentUsbDriver.currentText);
                }
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

    Component.onCompleted: {
        //console.log("make12222");
        UsbHelper.scanHD();
        //console.log("make11");
    }
}
