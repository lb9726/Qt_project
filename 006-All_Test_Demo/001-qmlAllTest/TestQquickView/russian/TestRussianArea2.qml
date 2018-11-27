import QtQuick 2.7
import QtQuick.Layouts 1.0
import QtQuick.Controls.Styles 1.3
import QtQuick.Controls 1.3

Item {
    property alias isDateTime: id_dateTimerChecker.checked
    property alias vTimeFormat: timeFormat_12.checked
    property alias vDateFormat: yearMouthDay.checked
    property bool isStandby: false/*id_standbyChecker.checked*/
    //    property alias isAudioSel: audio_swither.checked
    property bool isAudioSel: false
    //    property alias isAudioOff: audio_off.checked
    property bool isAudioOff: false
    //    property alias vStage1Interval: id_spinStage1Interval.value
    property int vStage1Interval: 0
    //    property alias vStage1Brightness: spinbox_bright_stage1.value
    property int vStage1Brightness: 0
    //    property alias vStage2Interval: id_spinStage2Interval.value
    property int vStage2Interval: 0
    //    property alias vStage2Brightness: spinbox_bright_stage2.value
    property int vStage2Brightness: 0

    property int hasChoiceAudioSwitchFlag: 0
    property int hasChoiceTimeAreaFlag: 0
    property bool autoSelectFlag: autoSelect.checked
    Text {
        id: id_Timeanddate_text1
        x: 0
        y: 0
        width: 72
        height: 30
        text: "время и дата"  // 时间和日期
        textFormat: Text.AutoText
    }

    Text {
        id: id_TimeFormat_text
        x: 30
        y: 36
        // text: qsTr("时间格式")
        text: "Формат времени"
    }

    Text {
        id: id_DateFormat_text
        x: 30
        y: 64
        // text: qsTr("日期格式")
        text: "Формат даты"
    }

    RadioButton {
        id: timeFormat_12
        enabled: !isDateTime
        ExclusiveGroup { id: timeFormatGroup }
        x: 252 + 25
        y: 34
        width: 89
        height: 16
        exclusiveGroup: timeFormatGroup
        // text: qsTr("12上午下午")
        text: qsTr("12 Часов")
    }

    RadioButton {
        id: timeFormat_24
        enabled: !isDateTime
        exclusiveGroup: timeFormatGroup
        checked: true
        x: 404 + 25
        y: 34
        width: 77
        height: 16
        text: "24 Часов" // text: qsTr("24小时")
    }

    RadioButton {
        id: yearMouthDay
        ExclusiveGroup { id: dateFormatGroup }
        enabled: !isDateTime
        checked: true
        x: 252 + 25
        y: 62
        width: 89
        height: 16
        exclusiveGroup: dateFormatGroup
        text: "yyyy:MM:dd"
    }

    RadioButton {
        id: mouthDayYear
        exclusiveGroup: dateFormatGroup
        enabled: !isDateTime
        x: 404 + 25
        y: 62
        text: "dd:MM:yyyy"
    }

    CheckBox {
        id: id_dateTimerChecker
        x: 0
        y: 98
        //text: qsTr("隐藏时间和日期")
        text: "Скрыть времни и даты"
        onClicked: {
            console.log("before hasChoiceTimeAreaFlag is ", hasChoiceTimeAreaFlag);
            hasChoiceTimeAreaFlag = 1;
            console.log("after hasChoiceTimeAreaFlag is ", hasChoiceTimeAreaFlag);
        }
    }

    ApnInformation {
        id: apnInformation
        enabled: !id_ApnSwitch.checked
        x: 0
        y: 131
        width: 592
        height: 54
    }

    CheckBox {
        id: id_ApnSwitch
        checked: true
        x: 0
        y: 187
        text: "СкрытиеApn" // 隐藏
    }

    Text {
        id: ipSettingTextId
        x: 0
        y: 216
        width: 41
        height: 12
        text: "IP настройки" // Ip设定
    }

    CheckBox {
        id: autoSelect
        checked: true
        x: 0
        y: 244
        ExclusiveGroup { id: selectGroup }
        exclusiveGroup: selectGroup
        text: "автоматически" // "自动"
    }

    CheckBox {
        id: manualSelect
        exclusiveGroup: selectGroup
        x: 0
        y: 270
        text: "вручную" // 手动
    }

    Text {
        id: ipAddressText
        enabled: !autoSelectFlag
        x: 0
        y: 309 + 15
        width: 50
        height: 16
        text: "IP-адрес:" // IP地址
    }

    IpEditRect {
        id: ipTextEdit
        enabled: !autoSelectFlag
        x: 262 + 15
        y: 290 + 20
        edit1Text: MediaScreen.getIndexIpString(1)
        edit2Text: MediaScreen.getIndexIpString(2)
        edit3Text: MediaScreen.getIndexIpString(3)
        edit4Text: MediaScreen.getIndexIpString(4)
    }

    Text {
        id: maskNetTextId
        enabled: !autoSelectFlag
        x: 0
        y: 350 + 15
        width: 50
        height: 16
        text: "Маска подсети:" // 子网掩码
    }

    IpEditRect {
        id: netTextEdit
        enabled: !autoSelectFlag
        x: 262 + 15
        y: 331 + 20
        edit1Text: qsTr("255")
        edit2Text: qsTr("255")
        edit3Text: qsTr("0")
        edit4Text: qsTr("0")
    }

    Text {
        id: defaultRouteTextId
        enabled: !autoSelectFlag
        x: 0
        y: 391 + 15
        width: 50
        height: 16
        text: "Шлюз по умолчанию:" // 默认网关
    }

    IpEditRect {
        id: routeTextEdit
        enabled: !autoSelectFlag
        x: 262 + 15
        y: 372 + 20
        edit1Text: "10"
        edit2Text: "200"
        edit3Text: "254"
        edit4Text: "0"
    }

    Text {
        id: dnsTextId
        enabled: !autoSelectFlag
        x: 0
        y: 433 + 15
        width: 50
        height: 16
        text: "DNS - сервис:" // 首选DNS服务
    }

    IpEditRect {
        id: dnsTextEdit
        enabled: !autoSelectFlag
        x: 262 + 15
        y: 418 + 20
        edit1Text: "10"
        edit2Text: "200"
        edit3Text: "0"
        edit4Text: "1"
    }

    function area2_parameter_timeDate()
    {
        if (!isDateTime)
        {
            ConfigureSerialer.updateDateTime(vTimeFormat ? "12" : "24",
                                                           vDateFormat? "yyyy.MM.dd":"dd.MM.yyyy");
        }
    }

    function area2_parameter_screenSaver()
    {
        if (isStandby)
        {
            ConfigureSerialer.updateStandby(vStage1Interval,vStage1Brightness,vStage2Interval,vStage2Brightness);
        }
    }

    function updateParameterSetting2() {
        var res = area1_briVolume.paraChecked();
        console.log(" updateParameterSetting2 res = ", res)
        if ("false" === res)
        {
            console.log(" updateParameterSetting2 res = ", res)
            if (!isDateTime && !isStandby)
            {
                console.log(" isDateTime = ", isDateTime, isStandby)
                ConfigureSerialer.updateParameterBasic2(false, !isDateTime,  isStandby, !isDateTime, isAudioSel, isAudioOff, hasChoiceTimeAreaFlag, hasChoiceAudioSwitchFlag)
            }
            else
            {
                console.log("false isDateTime = ", isDateTime, isStandby)
                ConfigureSerialer.updateParameterBasic2(true, !isDateTime,  isStandby, !isDateTime, isAudioSel, isAudioOff, hasChoiceTimeAreaFlag, hasChoiceAudioSwitchFlag)
            }
        }
        else
        {
            ConfigureSerialer.updateParameterBasic2(true, !isDateTime,  isStandby, !isDateTime, isAudioSel, isAudioOff, hasChoiceTimeAreaFlag, hasChoiceAudioSwitchFlag)
        }
        area2_parameter_timeDate();
        area2_parameter_screenSaver();
        ConfigureSerialer.setApnFlag(!id_ApnSwitch.checked);
        apnInformation.getAPNInformation();
        getIpInformation();
    }

    function resDate()
    {
        var resDateString = "";
        if(!isDateTime)
        {
            resDateString = (vDateFormat ? "YYYY:MM:DD" : "DD:MM:YYYY");
        }
        return resDateString;
    }

    function resTime()
    {
        var resTimeString = "";
        if(!isDateTime)
        {
            resTimeString = (vTimeFormat ? "00:00" : "00:00");
        }
        return resTimeString;
    }

    function getIpInformation()
    {
        var pIpAddress = ipTextEdit.getAddress();
        var pSubnetMask = netTextEdit.getAddress();
        var pDefaultGateWay = routeTextEdit.getAddress();
        var pDnsService = dnsTextEdit.getAddress();

        ConfigureSerialer.setIpParameter(autoSelect.checked, pIpAddress, pSubnetMask, pDefaultGateWay, pDnsService)
    }
}
