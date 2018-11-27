import QtQuick 2.7
import QtQuick.Layouts 1.0
import QtQuick.Controls.Styles 1.3
import QtQuick.Controls 1.3

Item {
    property string progressContent: ""
    Text {
        id: updateDiskMaking
        x: 0
        y: 0
        text: qsTr("升级盘制作")
    }

    Text {
        id: make_update_disk
        x: 0
        y: 40
        text: qsTr("制作升级盘")
//        font.pixelSize: 15
    }

    ComboBox {
        id: id_currentUsbDriver
        x: 148
        y: 32
        width: 114
        height: 26
        textRole: "key"
        enabled: id_driverModel.count > 0
        model: id_driverModel
    }

    CheckBox {
        id: reset_default
        x: 0
        y: 83
        text: qsTr("重置为默认配置")
        onCheckedChanged: {
            if (checked)
            {
                ConfigureSerialer.sendCheckedSignal();
                area1_briVolume.enabled = false;
                area2_timeandcycle.enabled = false;
                are3_multiUpdate.setEnableFlag(true);
            }
            else
            {
                area1_briVolume.enabled = true;
                area2_timeandcycle.enabled = true;
                are3_multiUpdate.setEnableFlag(false);
            }
        }
    }

    ButtonSelfDefine {
        id: btn_make
        x: 330
        y: 28
        bwidth: 66
        bheight: 36
        btext: qsTr("制作")
        enabled: id_driverModel.count > 0 && id_currentUsbDriver.currentText != ""
        onButtonClick: {
            if("" != id_currentUsbDriver.currentText)
            {
                console.log("will call makeDisk()");
                makeDisk();
            }
        }
    }

    function makeDisk()
    {
        var ret;
//        ret = parameterView1.checkParameterSetting();
//        console.log("before ret = ", ret);
//        if(ret !== "" && !informationWarning.visible) {
//            informationContent.text = ret;
//            console.log("area1 checked!")
//            if (reset_default.checked)
//            {
//                informationWarning.visible = false;
//            }
//            else
//            {
//                informationWarning.visible = true;
//                return;
//            }
//        }

        ret = multiMediaView.checkMediaSetting();
        console.log("before ret = ", ret);
        if(ret !== "" && !informationWarning.visible)
        {
            console.log("area3 checked!");
            informationContent.text = ret;
            if (reset_default.checked)
            {
                informationWarning.visible = false;
            }
            else
            {
                informationWarning.visible = true;
                return;
            }
        }

        ConfigureSerialer.clearParameters();//清除上次的配置参数

        if(reset_default.checked)
        {
            ConfigureSerialer.resetScreen(true);
        }
        else
        {
            console.log("will update parametersetting");
            multiMediaView.updateMediaSetting();
            parameterView1.updateParameterSetting1();
            parameterView2.updateParameterSetting2();
        }

        id_mediaScreenHelper.format_text();
        id_mediaScreenHelper.setprogressOn();
        //        UsbHelper.formatUdisk(id_currentUsbDriver.currentText);
        ConfigureSerialer.setDelDirPath(id_currentUsbDriver.currentText);
        ConfigureSerialer.startDelDir();
        id_copyTimer.start();
        //        id_mediaScreenHelper.donotFormatUdisk();
    }

    function checkIsShowPreview()
    {
        var ret;
//        ret = parameterView1.checkParameterSetting();
//        console.log("before ret = ", ret);
//        if(ret !== "" && !informationWarning.visible) {
//            informationContent.text = ret;
//            console.log("area1 checked!")
//            if (reset_default.checked)
//            {
//                informationWarning.visible = false;
//            }
//            else
//            {
//                informationWarning.visible = true;
//                return;
//            }
//        }

        ret = multiMediaView.checkMediaSetting();
        console.log("before ret = ", ret);
        if(ret !== "" && !informationWarning.visible)
        {
            console.log("area3 checked!");
            informationContent.text = ret;
            if (reset_default.checked)
            {
                informationWarning.visible = false;
            }
            else
            {
                informationWarning.visible = true;
                return;
            }
        }
        open_horizontal_view.getAllResource(area1_briVolume.titleString(), area1_briVolume.scrollString(),
            area2_timeandcycle.resDate(), area2_timeandcycle.resTime(),
            are3_multiUpdate.videoIsCheck(), are3_multiUpdate.audioIsCheck(), are3_multiUpdate.pictureIsCheck(),
            are3_multiUpdate.videoSelectPath(), are3_multiUpdate.pictureCounts(),
            are3_multiUpdate.pictureSelectPath(), are3_multiUpdate.cycleSeconds(),are3_multiUpdate.getIsfullScreen());
        open_vertical_view.getAllResource(area1_briVolume.titleString(), area1_briVolume.scrollString(),
            area2_timeandcycle.resDate(), area2_timeandcycle.resTime(),
            are3_multiUpdate.videoIsCheck(), are3_multiUpdate.audioIsCheck(), are3_multiUpdate.pictureIsCheck(),
            are3_multiUpdate.videoSelectPath(), are3_multiUpdate.pictureCounts(),
            are3_multiUpdate.pictureSelectPath(), are3_multiUpdate.cycleSeconds(),are3_multiUpdate.getIsfullScreen());
        ConfigureSerialer.sendShowPreview();
    }

    function area5_orderfile()
    {
        console.log("area5_orderfile()");
        ConfigureSerialer.orderFile(id_currentUsbDriver.currentText);
    }

    function are5_DiskPath()
    {
        var res = "";
        res = id_currentUsbDriver.currentText;
        return res;
    }

    Connections {
        target: UsbHelper
        onSigDeviceAdd: {
            id_driverModel.append({"key": dir});
            console.log("qml recieve deviceAdd signal");
            MediaScreen.setUdiskIsInsertFlag(true, dir);
        }
        onSigDeviceRemove: {
            console.log("qml is recieve deviceRemove signal");
            console.log("qml id_driverModel.count = ", id_driverModel.count);
            for(var i = 0; i < id_driverModel.count; ++i)
            {
                console.log(id_driverModel.get(i).key);
                if(id_driverModel.get(i).key === dir)
                {
                    id_driverModel.remove(i);
                    MediaScreen.setUdiskIsInsertFlag(false, dir);
                }
            }
        }
    }
    ListModel {
        id: id_driverModel
    }

    Connections {
        target: Ctranslator
        onLanguageChanges: {
            changesLanguge();
        }
    }
    function changesLanguge()
    {
        console.log("语言改变 Area5")
        updateDiskMaking.text = qsTr("升级盘制作")
        reset_default.text = qsTr("重置为默认配置")
        make_update_disk.text = qsTr("制作升级盘")
        btn_make.text = qsTr("制作")
        progressContent = qsTr("拷贝文件...")
    }

    function getResetDefault()
    {
        var res = "";
        if (reset_default.checked)
        {
            res = true;
            return res;
        }
        else
        {   res = false;
            return res;
        }
    }

    Connections {
        target: ConfigureSerialer
        onResetCanceled: {
            reset_default.checked = false;
        }
    }

    Connections {
        target: MediaScreen
        onHasUdiskInsertToComputer: {
            if (3 !== MediaScreen.getIndex())
            {
                id_driverModel.append({"key": uDiskName});
                console.log("is Russian add UdiskInformation")
            }
        }
    }

    Component.onCompleted: {
        UsbHelper.scanHD();
    }
}
