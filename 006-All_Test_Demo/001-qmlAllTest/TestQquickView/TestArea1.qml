import QtQuick 2.7
import QtQuick.Layouts 1.0
import QtQuick.Controls.Styles 1.3
import QtQuick.Controls 1.3

Item {
    property alias vBrightness: spinbox_bright.value
    property alias vVolume: spinbox_volume.value
    //    property alias isBrightnessVolume: id_briVolumeChecker.checked
    property bool isBrightnessVolume: false

    property alias isScrollText: id_scrollintTextChecker.checked
    property alias vScrollText: scrolltext.text

    property alias isTitle: id_titleChecker.checked
    property alias vTitle: titletext.text
    property int hasChoiceScrollTextFlag: 0
    property int hasChoiceTitleShowFlag: 0

    property int hasChoiceAudioSwitchFlag: 0
    property int hasChoiceTimeAreaFlag: 0

    property int brightChanged: 0
    property int volumeChanged: 0

    Text {
        id: id_briVolumeText
        x: 0
        y: 0
        text: qsTr("屏幕设定")
//        font.pixelSize: 12
    }

    Text {
        id: id_bright_text
        x: 33
        y: 27
        text: qsTr("亮度")
    }

    Slider {
        id: slider_bright
        x: 136
        y: 28
        minimumValue: 0
        maximumValue: 100
        value: 0
        onValueChanged: {
            spinbox_bright.value = value
            brightChanged = 1;
        }
        style: SliderStyle {
            groove: Rectangle {
                color: "darkGray"
                implicitHeight: 6
                implicitWidth: 100
                opacity: 0.8
                radius: 3
                BorderImage {
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/image/greenslider.png"
                    border.left: 5; border.top: 1
                    border.right: 5; border.bottom: 1
                    width: styleData.handlePosition
                    height: 5
                }
            }
            handle: Item {
                width: 13
                height: 13
                Image {
                    anchors.centerIn: parent
                    source: "qrc:/image/handle.png"
                }
            }
        }
    }

    Text {
        id: id_volume_text
        x: 33
        y: 60
        text: qsTr("音量")
    }

    Slider {
        id: slider_volume
        x: 136
        y: 61
        minimumValue: 0
        maximumValue: 100
        value: 0
        onValueChanged: {
            spinbox_volume.value = value;
            volumeChanged = 1;
        }
        style: SliderStyle {
            groove: Rectangle {
                color: "darkGray"
                implicitHeight: 6
                implicitWidth: 100
                opacity: 0.8
                radius: 3
                BorderImage {
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/image/greenslider.png"
                    border.left: 5; border.top: 1
                    border.right: 5; border.bottom: 1
                    width: styleData.handlePosition
                    height: 5
                }
            }
            handle: Item {
                width: 13
                height: 13
                Image {
                    anchors.centerIn: parent
                    source: "qrc:/image/handle.png"
                }
            }
        }
    }

    SpinBox {
        id: spinbox_bright
        x: 294
        y: 21
        width: 60
        height: 25
        minimumValue: 0
        maximumValue: 100
        value: 0
        onValueChanged: {
            slider_bright.value = value
            brightChanged = 1;
        }
    }

    SpinBox {
        id: spinbox_volume
        x: 294
        y: 54
        width: 60
        height: 25
        minimumValue: 0
        maximumValue: 100
        value: 0
        onValueChanged: {
            slider_volume.value = value
            volumeChanged = 1;
        }
    }

    Text {
        id: id_aduio_text
        x: 0
        y: 96
        text: qsTr("音频")
    }

    CheckBox {
        id: audio_swither
        ExclusiveGroup { id: audioGroup }
        exclusiveGroup: audioGroup
        x: 49
        y: 117
        text: qsTr("开")
        onClicked: {
            console.log("before hasChoiceAudioSwitchFlag is ", hasChoiceAudioSwitchFlag)
            hasChoiceAudioSwitchFlag = 1;
            console.log("after hasChoiceAudioSwitchFlag is ", hasChoiceAudioSwitchFlag)
        }
    }

    CheckBox {
        id: audio_off
        exclusiveGroup: audioGroup
        x: 140
        y: 117
        text: qsTr("关")
    }

    Text {
        id: setScrollText_id
        x: 0
        y: 158
        text: qsTr("设置滚动文字")
    }

    TextField {
        id: scrolltext
        x: 49
        y: 184
        enabled: !id_scrollintTextChecker.checked
        placeholderText: qsTr("请输入字幕文字")
        font.family: "KONE Information_v12"
        style: TextFieldStyle { // 自定义样式
            background: Rectangle {
                radius: 1
                implicitWidth: 260 + 45
                implicitHeight: 24
                border.color: "green"
                border.width: 1
            }
        }
    }

    CheckBox {
        id: id_scrollintTextChecker
        x: 0
        y: 224
        text: qsTr("隐藏滚动文字")
        onClicked: {
            console.log("before hasChoiceScrollTextFlag is ", hasChoiceScrollTextFlag)
            hasChoiceScrollTextFlag = 1;
            console.log("after hasChoiceScrollTextFlag is ", hasChoiceScrollTextFlag)
        }
    }

    Text {
        id: setTitle_id
        x: 0
        y: 257
        text: qsTr("设置标题")
    }

    TextField {
        id: titletext
        x: 49
        y: 284
        enabled: !id_titleChecker.checked
        placeholderText: qsTr("请输入标题文字")
        font.family: "KONE Information_v12"
        style: TextFieldStyle {
            background: Rectangle {
                radius: 1
                implicitWidth: 305
                implicitHeight: 24
                border.color: "green"
                border.width: 1
            }
        }
    }

    CheckBox {
        id: id_titleChecker
        x: 0
        y: 324
        text: qsTr("隐藏标题")
        onClicked: {
            console.log(" before hasChoiceTitleShowFlag is ", hasChoiceTitleShowFlag)
            hasChoiceTitleShowFlag = 1;
            console.log("after hasChoiceTitleShowFlag is ", hasChoiceTitleShowFlag)
        }
    }

    Text {
        id: systemTime
        x: 0
        y: 363
        text: qsTr("系统时间设置")
    }

    Self_TextField {
        id: systimeEditText
        enabled: !id_systimeSwitch.checked
        x: 32
        y: 388
        yeartext: MediaScreen.getSplitDate(1);
        monthtext: MediaScreen.getSplitDate(2);
        daytext: MediaScreen.getSplitDate(3);
        hourtext: MediaScreen.getSplitTime(1);
        minutetext: MediaScreen.getSplitTime(2);
        secondtext: MediaScreen.getSplitTime(3);
    }

    CheckBox {
        id: id_systimeSwitch
        checked: true
        x: 0
        y: 448
        text: qsTr("隐藏系统时间")
    }

    Connections {
        target: Ctranslator
        onLanguageChanges: {
            changesLanguge();
        }
    }

    function changesLanguge()
    {
        console.log("语言改变 Area1")
//        id_briVolumeChecker.text = qsTr("亮度&音量")
        id_briVolumeText.text = qsTr("屏幕设定")
        id_bright_text.text = qsTr("亮度")
        id_volume_text.text = qsTr("音量")
        id_aduio_text.text = qsTr("音频")
        audio_swither.text = qsTr("开");
        audio_off.text = qsTr("关")
//        scrollText_id.text = qsTr("滚动文字")
        setScrollText_id.text = qsTr("设置滚动文字")
        setTitle_id.text = qsTr("设置标题")
//        if (id_scrollintTextChecker.checked)
//        {
//            id_scrollintTextChecker.text = qsTr("显示滚动文字")
//        }
//        else
//        {
//            id_scrollintTextChecker.text = qsTr("隐藏滚动文字")
//        }
        id_scrollintTextChecker.text = qsTr("隐藏滚动文字")
//        if (id_titleChecker.checked)
//        {
//            id_titleChecker.text = qsTr("显示标题")
//        }
//        else
//        {
//            id_titleChecker.text = qsTr("隐藏标题")
//        }
        id_titleChecker.text = qsTr("隐藏标题")
        scrolltext.placeholderText = qsTr("请输入字幕文字")
//        title_id.text = qsTr("标题")
        titletext.placeholderText = qsTr("请输入标题文字")
        systemTime.text = qsTr("系统时间设置")
        systimeEditText.translateText();
        id_systimeSwitch.text = qsTr("隐藏系统时间")
    }

    function checkParameterSetting()
    {
        if (!isScrollText && !isTitle)
        {
            return ""
        }

        if(isScrollText && vScrollText == "")
        {
            return qsTr("请输入滚动字幕文字")
        }

        if(isTitle && vTitle == "")
        {
            return qsTr("请输入标题文字")
        }
        return "";
    }

    function paraChecked()
    {
        if (!isBrightnessVolume && isScrollText && isTitle)
        {
            return "false";
        }
        else
        {
            return "true";
        }
    }

    function updateParameterSetting1()
    {
        console.log("updateParameterSetting1 is call")
        if (isBrightnessVolume && isScrollText && isTitle)
        {
            console.log("full false")
            ConfigureSerialer.updateParameterBasic1(false, false, false, false, false, false, hasChoiceScrollTextFlag, hasChoiceTitleShowFlag);
        }
        else
        {
            console.log("not full false")
            ConfigureSerialer.updateParameterBasic1(true, isBrightnessVolume, !isScrollText, !isTitle, !isScrollText, !isTitle, hasChoiceScrollTextFlag, hasChoiceTitleShowFlag)
        }

        if(isBrightnessVolume)
        {
            console.log("isBrightnessVolume = ", isBrightnessVolume);
            ConfigureSerialer.updateBrightnessVolume(vBrightness, vVolume);
        }

        if(!isScrollText)
        {
            ConfigureSerialer.updateScrollText(vScrollText);
        }

        if(!isTitle)
        {
            ConfigureSerialer.updateTitle(vTitle);
        }
        ConfigureSerialer.updateArea1ParaAudio(audio_swither.checked, audio_off.checked);
        brightOrVolumeChanged();
        ConfigureSerialer.setSystimeText(systimeEditText.getEditTime(), !id_systimeSwitch.checked)
    }

    function titleString()
    {   var resString = "";
        if (!isTitle)
        {
            var i = ConfigureSerialer.judgeString(titletext.text);
            if (0 === i || 1 === i) // 为空或者全为空格时，显示默认标题
            {
                resString = "KONE";
            }
            else if (2 === i)
            {
                resString = titletext.text;
            }
        }
        return resString;
    }

    function scrollString()
    {
        var resScroll = "";
        if (!isScrollText)
        {
            var i = ConfigureSerialer.judgeString(scrolltext.text);
            if (0 === i || 1 === i) // 为空或者全为空格时，显示默认标题
            {
                resScroll = "Have a nice day! Have a nice day!";
            }
            else if (2 === i)
            {
                resScroll = scrolltext.text;
            }
        }
        return resScroll;
    }

    function brightOrVolumeChanged()
    {
        if (1 === brightChanged || 1 === volumeChanged)
        {
            ConfigureSerialer.setBrightOrVolumeChanged(1)
            console.log("the brightOrVolumeChanged() is true")
            ConfigureSerialer.updateBrightnessVolume(vBrightness, vVolume);
        }
        else
        {
            ConfigureSerialer.setBrightOrVolumeChanged(0);
            console.log("the brightOrVolumeChanged() is false")
        }
    }
}
