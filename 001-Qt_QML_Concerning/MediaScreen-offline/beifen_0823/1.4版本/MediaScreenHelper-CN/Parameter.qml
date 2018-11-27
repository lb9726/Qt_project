import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Page {
    property alias bConfigureParameter: id_configureSwitcher.checked

    property alias bBrightnessVolume: id_BriVolSwitcher.checked
    property alias bScrollText: id_scrollTextSwitcher.checked
    property alias bTitle: id_TextSwitcher.checked
    property alias bDateTime: id_DateSwitcher.checked
    property alias bStandby: id_StandbySwitcher.checked

    onBBrightnessVolumeChanged: {
        if(!bBrightnessVolume){
            isBrightnessVolume = false;
        }
    }

    onBScrollTextChanged: {
        if(!bScrollText){
            //console.log("h 1")
            isScrollText  = false;
        }else{
            //console.log("h 2")
        }
    }

    onBTitleChanged: {
        if(!bTitle)
            isTitle  = false;
    }

    onBDateTimeChanged: {
        if(!bDateTime)
            isDateTime  = false;
    }

    onBStandbyChanged: {
        if(!bStandby)
            isStandby  = false;
    }

    property alias isBrightnessVolume: id_briVolumeChecker.checked
    property alias vVolume:id_spinVolume.value
    property alias vBrightness:id_spinBrightness.value

    property alias isScrollText: id_scrollintTextChecker.checked
    property alias vScrollText:id_scrollintText.text

    property alias isTitle: id_titleChecker.checked
    property alias vTitle:id_titleText.text

    property alias isDateTime: id_dateTimerChecker.checked
    property alias vTimeFormat:id_TimeFormatRadio.checked
    property alias vDateFormat:id_DateFormatRadio.checked

    property alias isStandby: id_standbyChecker.checked
    property alias vStage1Interval:id_spinStage1Interval.value
    property alias vStage1Brightness:id_spinStage1Brightness.value
    property alias vStage2Interval:id_spinStage2Interval.value
    property alias vStage2Brightness:id_spinStage2Brightness.value


    header: ToolBar {
        RowLayout {
            anchors.fill: parent
            SwitchHelper{
                id:id_configureSwitcher
                text: checked ? qsTr("开") :qsTr("关")
            }
            Switch {
                id: id_BriVolSwitcher
                text: qsTr("亮度&音量")
                visible: bConfigureParameter
            }
            Switch {
                id: id_scrollTextSwitcher
                text: qsTr("滚动文字")
                visible: bConfigureParameter
            }
            Switch {
                id: id_TextSwitcher
                text: qsTr("标题")
                visible: bConfigureParameter
            }
            Switch {
                id: id_DateSwitcher
                text: qsTr("日期&时间")
                visible: bConfigureParameter
            }
            Switch {
                id: id_StandbySwitcher
                text: qsTr("屏保")
                visible: bConfigureParameter
            }
        }
    }

    RowLayout{
        anchors.centerIn: parent
        visible: bConfigureParameter
        ColumnLayout{
            //width: 500
            //anchors.top: parent.top
            ColumnLayout {
                width: parent.width
                visible: id_BriVolSwitcher.checked
                CheckBox {
                    id: id_briVolumeChecker
                    text: qsTr("设置亮度&音量")
                }
                RowLayout{
                    visible: id_briVolumeChecker.checked
                    height: 50
                    Text{
                        text: qsTr("亮度")
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    Slider {
                        id: id_sliderBrightness
                        width: 100
                        height: 40
                        from: 0
                        to:100
                        value: id_spinBrightness.value
                        onValueChanged: {
                            if(value != id_spinBrightness.value){
                                id_spinBrightness.value = parseInt(value);
                            }
                        }
                    }
                    SpinBox{
                        id:id_spinBrightness
                        width: 100
                        value: 80
                        from: 0
                        to:100
                    }
                }
                RowLayout{
                    visible: id_briVolumeChecker.checked
                    height: 50
                    Text{
                        text: qsTr("音量")
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Slider {
                        id: id_sliderVolume
                        width: 100
                        height: 40
                        from: 0
                        to:100
                        value: id_spinVolume.value
                        onValueChanged: {
                            if(value != id_spinVolume.value){
                                id_spinVolume.value = parseInt(value);
                            }
                        }
                    }
                    SpinBox{
                        id:id_spinVolume
                        width: 100
                        value: 60
                        from: 0
                        to:100
                    }
                }
            }

            ColumnLayout{
                visible: id_scrollTextSwitcher.checked
                width: parent.width
                CheckBox {
                    id: id_scrollintTextChecker
                    text: checked ? qsTr("显示滚动字幕"):qsTr("隐藏滚动字幕")
                }
                TextField {
                    id: id_scrollintText
                    visible: id_scrollintTextChecker.checked
                    placeholderText: qsTr("请输入字幕文字")
                    maximumLength: 64
                    width: contentWidth > 400 ? 400 : contentWidth
                }
            }

            CheckBox {
                id: id_standbyChecker
                text: qsTr("设置屏保")
                visible: id_StandbySwitcher.checked
            }

            ColumnLayout {
                width: parent.width
                visible: id_standbyChecker.checked && id_StandbySwitcher.checked
                RowLayout{
                    width: parent.width
                    height: 50

                    Text{
                        text: qsTr("一阶段 \n周期(s):")
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    SpinBox{
                        id:id_spinStage1Interval
                        width: 50
                        value: 90
                        from: 0
                        to:100
                    }
                }
                RowLayout{
                    width: parent.width
                    height: 50

                    Text{
                        text: qsTr("亮度:")
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Slider {
                        id: id_sliderStage1
                        width: 100
                        height: 40
                        from: 0
                        to:100
                        value: id_spinStage1Brightness.value
                        onValueChanged: {
                            if(value != id_spinStage1Brightness.value){
                                id_spinStage1Brightness.value = parseInt(value);
                            }
                        }
                    }
                    SpinBox{
                        id:id_spinStage1Brightness
                        width: 50
                        value: 15
                        from: 0
                        to:100
                    }
                }
                RowLayout{
                    width: parent.width
                    height: 50
                    Text{
                        text: qsTr("二阶段 \n周期(s):")
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    SpinBox{
                        id:id_spinStage2Interval
                        width: 50
                        value: 1200
                        from: 0
                        to:3600
                    }
                }
                RowLayout{
                    width: parent.width
                    height: 50
                    Text{
                        text: qsTr("亮度")
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Slider {
                        id: id_sliderStage2
                        width: 100
                        height: 40
                        from: 0
                        to:100
                        value: id_spinStage2Brightness.value
                        onValueChanged: {
                            if(value != id_spinStage2Brightness.value){
                                id_spinStage2Brightness.value = parseInt(value);
                            }
                        }
                    }
                    SpinBox{
                        id:id_spinStage2Brightness
                        width: 100
                        value: 0
                        from: 0
                        to:100
                    }
                }
            }
        }

        ColumnLayout{
            //width: 500
            //anchors.top: parent.top
            ColumnLayout {
                id: id_dateTimeArea
                visible: id_DateSwitcher.checked
                width: parent.width
                CheckBox {
                    id: id_dateTimerChecker
                    text: checked ? qsTr("显示日期和时间"):qsTr("隐藏日期和时间")
                }

                RowLayout {
                    width: 100
                    visible: id_dateTimerChecker.checked
                    ColumnLayout {
                        width: 100
                        Label {
                            id: label4
                            text: qsTr("时间格式")
                            anchors.horizontalCenter: parent.horizontalCenter
                        }

                        RadioButton {
                            id: id_TimeFormatRadio
                            checked: true
                            text: qsTr("12 上午/下午")
                        }

                        RadioButton {
                            id: radioButton1
                            text: qsTr("24 小时")
                        }
                    }

                    ColumnLayout {
                        width: 100
                        Label {
                            text: qsTr("日期格式")
                        }

                        RadioButton {
                            id: id_DateFormatRadio
                            checked: true
                            text: qsTr("yyyy.mm.dd")
                        }

                        RadioButton {
                            text: qsTr("mm.dd.yyyy")
                        }
                    }
                }
            }
            ColumnLayout{
                visible: id_TextSwitcher.checked
                width: parent.width
                CheckBox {
                    id: id_titleChecker
                    text: checked ? qsTr("显示标题"): qsTr("隐藏标题")
                }

                TextField {
                    id: id_titleText
                    visible: id_titleChecker.checked
                    placeholderText: qsTr("请输入标题文字")
                    maximumLength: 18
                    width: contentWidth > 400 ? 400 : contentWidth
                }
            }
        }
    }

    function checkParameterSetting(){
        if(!bConfigureParameter)
            return "";

        if(!bBrightnessVolume&&!bScrollText&&!bTitle&&!bDateTime&&!bStandby){
            return "";
        }

        if(isScrollText && vScrollText == ""){
            return qsTr("请输入滚动字幕文字")
        }

        if(isTitle && vTitle == ""){
            return qsTr("请输入标题文字")
        }

        return "";
    }

    function updateParameterSetting(){
        if(!bConfigureParameter){
            ConfigureSerialer.updateParameterBasic(false,false,false,false,false,false,false,false,false);
            return;
        }else{
            //bool scroll = bScrollText&&isScrollText,
            ConfigureSerialer.updateParameterBasic(true,
                                                   bBrightnessVolume&&isBrightnessVolume,
                                                   bScrollText,
                                                   bTitle,
                                                   bDateTime,
                                                   bStandby&&isStandby,
                                                   isScrollText,
                                                   isTitle,
                                                   isDateTime);

//            ConfigureSerialer.updateParameterBasic(true,
//                                                   bBrightnessVolume&&isBrightnessVolume,
//                                                   bScrollText&&isScrollText,
//                                                   bTitle&&isTitle,
//                                                   bDateTime&&isDateTime,
//                                                   bStandby&&isStandby);

        }

        if(bBrightnessVolume&&isBrightnessVolume){
            ConfigureSerialer.updateBrightnessVolume(vBrightness,vVolume);
        }

        if(bScrollText&&isScrollText){
            ConfigureSerialer.updateScrollText(vScrollText);
        }

        if(bTitle&&isTitle){
            ConfigureSerialer.updateTitle(vTitle);
        }

        if(bDateTime&&isDateTime){
            ConfigureSerialer.updateDateTime(vTimeFormat ? "12" : "24",
                                            vDateFormat?"yyyy.mm.dd":"mm.dd.yyyy");
        }

        if(bStandby&&isStandby){
            ConfigureSerialer.updateStandby(vStage1Interval,vStage1Brightness,vStage2Interval,vStage2Brightness);
        }
    }
}
