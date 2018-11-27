import QtQuick 2.7
//import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls.Styles 1.3
import QtQuick.Controls 1.3
Item {
    Item {
        width: parent.width
        height: 20
        id: updatedisk_id
        Label {
            text: qsTr("升级盘制作")
            anchors.centerIn: parent
        }
    }
    ColumnLayout {
        spacing: 20
        RowLayout {

        }
        RowLayout {
            Rectangle {
                color: "transparent"
                width: 50
            }

            CheckBox {
                id: reset_default
                text: qsTr("重置为默认配置")
            }
        }
        RowLayout {
            spacing: 50
            Rectangle {
                color: "transparent"
                width: 50
            }
            Text {
                id: make_update_disk
                text: qsTr("制作升级盘")
            }
            ComboBox {
                id:id_currentUsbDriver
                width: 10
                height: 50
                textRole: "key"
                enabled: id_driverModel.count > 0
                model: id_driverModel
            }
            Button {
                id: btn_make
                text: qsTr("制作")
//                background:
                enabled: id_driverModel.count > 0 && id_currentUsbDriver.currentText !=""
                onClicked: {
                    if(id_currentUsbDriver.currentText != ""){
                        countDownTimes = 0;
                        makeDisk();
                    }else{

                    }
                }
                style: ButtonStyle {
                    background: BorderImage {
                        source: btn_make.pressed ? "qrc:///image/Brown.png":"qrc:///image/blue.png"
                        width: btn_make.width; height: btn_make.height
                        border.left: 5; border.top: 5
                        border.right: 5; border.bottom: 5
                    }
                    label:Rectangle {
                        color: "transparent"
                        Text{
//                            width: btn.width; height:btn.height
                            text: btn_make.text
                            color:"white"
                            anchors.centerIn: parent
                            font.family: "KONE Information_v12"
//                            font.pixelSize:12
//                            maximumLineCount:1   //设置最大最行数
//                            elide:Text.ElideLeft
//                            wrapMode:Text.Wrap
                        }
                    }
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

        progressContent = qsTr("正在格式化U盘...")
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
                    progressContent = qsTr("拷贝文件...")
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
        progressContent = qsTr("正在生成配置参数");
        ConfigureSerialer.createXml(id_currentUsbDriver.currentText);
        progressContent = qsTr("升级盘制作成功");
        showFinishedButton = true;
    }

    property bool enterFlag: false
    Connections {
        target: ConfigureSerialer
        onCopyFileProgress:{
            fileProgress = pro;
            if(pro == 100 && enterFlag == false){
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
    ListModel {
        id:id_driverModel
        //ListElement { key: "First"}
        //ListElement { key: "Second"}
        //ListElement { key: "Third"}
    }
    Component.onCompleted: {
        UsbHelper.scanHD();
    }
}
