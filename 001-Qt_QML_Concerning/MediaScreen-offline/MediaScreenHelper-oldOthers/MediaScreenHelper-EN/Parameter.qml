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
            console.log("h 1")
            isScrollText  = false;
        }else{
            console.log("h 2")
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
                text: checked ? qsTr("ON") :qsTr("OFF")
            }
            Switch {
                id: id_BriVolSwitcher
                text: qsTr("Brightness&Volume")
                visible: bConfigureParameter
            }
            Switch {
                id: id_scrollTextSwitcher
                text: qsTr("Scrolling Text")
                visible: bConfigureParameter
            }
            Switch {
                id: id_TextSwitcher
                text: qsTr("Title")
                visible: bConfigureParameter
            }
            Switch {
                id: id_DateSwitcher
                text: qsTr("Date&Time")
                visible: bConfigureParameter
            }
            Switch {
                id: id_StandbySwitcher
                text: qsTr("Standby")
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
                    text: qsTr("Configure Brightness & Volume")
                }
                RowLayout{
                    visible: id_briVolumeChecker.checked
                    height: 50
                    Text{
                        text: qsTr("Brightness")
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
                        text: qsTr("Volume")
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
                    text: checked ? qsTr("Show Scrolling Text"):qsTr("Hide Scrolling Text")
                }
                TextField {
                    id: id_scrollintText
                    visible: id_scrollintTextChecker.checked
                    placeholderText: qsTr("Enter scrolling text")
                    maximumLength: 64
                    width: contentWidth > 400 ? 400 : contentWidth
                }
            }

            CheckBox {
                id: id_standbyChecker
                text: qsTr("Configure Standby")
                visible: id_StandbySwitcher.checked
            }

            ColumnLayout {
                width: parent.width
                visible: id_standbyChecker.checked && id_StandbySwitcher.checked
                RowLayout{
                    width: parent.width
                    height: 50

                    Text{
                        text: qsTr("Stage One \nInterval(s):")
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
                        text: qsTr("Brightness:")
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
                        text: qsTr("Stage Two \nInterval(s):")
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
                        text: qsTr("Brightness")
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
                    text: checked ? qsTr("Show Date & Time"):qsTr("Hide Date & Time")
                }

                RowLayout {
                    width: 100
                    visible: id_dateTimerChecker.checked
                    ColumnLayout {
                        width: 100
                        Label {
                            id: label4
                            text: qsTr("Time Format")
                            anchors.horizontalCenter: parent.horizontalCenter
                        }

                        RadioButton {
                            id: id_TimeFormatRadio
                            checked: true
                            text: qsTr("12 am/pm Hours")
                        }

                        RadioButton {
                            id: radioButton1
                            text: qsTr("24 Hours")
                        }
                    }

                    ColumnLayout {
                        width: 100
                        Label {
                            text: qsTr("Date Format")
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
                    text: checked ? qsTr("Show Title"): qsTr("Hide Title")
                }

                TextField {
                    id: id_titleText
                    visible: id_titleChecker.checked
                    placeholderText: qsTr("Enter title text")
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
            return "Please enter the scrolling text"
        }

        if(isTitle && vTitle == ""){
            return "Please enter the title text"
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
