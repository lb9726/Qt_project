import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls.Material 2.0

Popup {
    property string vborder_Color: "grey";
    property string hborder_Color: "red";
    property bool isFullScreen: false;
    id: id_resetWarnningBox
    x: (id_mediaScreenHelper.width - width)/2
    y: (id_mediaScreenHelper.height - height)/2
    width: 530
    height: 687
    modal: true
    focus: true
    closePolicy: Popup.NoAutoClose
    Button_Circle {
        x: parent.width - 20
        y: -5
        onButtonclick: {
            vertical_layout.stopVideo();
            vertical_layout_fullscreen.stopVideo();
            open_vertical_view.close();   
//            ConfigureSerialer.sendPreviewCheckedFalse();
            ConfigureSerialer.sendStopAudioPlay();
        }
    }
    Item {
        x: 0
        y: 0
        width: 94
        height: 46
        Text {
            x: 0
            y: -5
            id: layout_preview
            text: qsTr("界面预览")
//            anchors.centerIn: parent
        }
    }
    Rectangle {
        id: greyline
        x: /*13*/ 2
        y: 46
        width: 515
        height: 2
        color: "grey"
    }
    Rectangle {
        x: 6
        y: 63
        id: hRect
        width: btn_horizatal.width + 4
        height: btn_horizatal.height + 4
//        border.color: hborder_Color
//        border.width: 1
        Button_Self_Inactive {
            id: btn_horizatal
            anchors.centerIn: parent
            checkable: true
//            visible: !isFullScreen
            btext: qsTr("横屏显示")
            bwidth: 238
            bheight: 41
            onButtonClick:
            {
                vertical_layout.stopVideo();
                vertical_layout_fullscreen.stopVideo();
                open_vertical_view.close();
                open_horizontal_view.open();
                ConfigureSerialer.sendHorizontalAutoPlay(isFullScreen)
                ConfigureSerialer.sendStartPicAnimation();
            }
        }
    }

    Rectangle {
        x: 260
        y: 63
        id: vRect
        width:btn_vertical.width + 4
        height: btn_vertical.height + 4
//        border.color: vborder_Color
//        border.width: 1
//        Button_Self {
//            id: btn_vertical
//            anchors.centerIn: parent
//            checkable: true;
//            checked: true;
//            enabled: false
////            visible: !isFullScreen
//            text: qsTr("竖屏显示")
//            onClicked:
//            {
//                open_horizontal_view.close();
//                open_vertical_view.open();
//            }
//        }
        ButtonSelfDefine {
            id: btn_vertical
            anchors.centerIn: parent
            checkable: true;
            checked: true;
            enabled: false
            btext: qsTr("竖屏显示")
            bwidth: 238
            bheight: 41
            onButtonClick: {
                console.log("is ButtonSelfDefine modifyed")
                open_horizontal_view.close();
                open_vertical_view.open();
            }
        }
    }
    Connections {
        target: ConfigureSerialer
        onIsFullScreenChanges: {
            isFullScreen = isfull;
        }
    }

    Layout_Vertical
    {
        x: 65
        y: 135
        id: vertical_layout
        visible: !isFullScreen
    }
    Layout_Vertical_FullScreen
    {
        x: 65
        y: 135
        id: vertical_layout_fullscreen
        visible: isFullScreen
    }

    Connections {
        target: Ctranslator
        onLanguageChanges: {
            layoutChangeLan();
        }
    }

    function layoutChangeLan()
    {
        console.log("语言改变 layoutHorizontal")
        layout_preview.text = qsTr("界面预览")
        btn_horizatal.text = qsTr("横屏显示")
        btn_vertical.text = qsTr("竖屏显示")
    }

    function getAllResource(title, scrolltext, date, time, isvideo, isaudio, ispic, videop, picnum, picp, picInterval, isfull)
    {
        if (true === isfull)
        {
            console.log("getAllResource in verticalImageorvideo")
            vertical_layout_fullscreen.setTitleText(title);
            vertical_layout_fullscreen.setScrollText(scrolltext);
            vertical_layout_fullscreen.setDateText(date);
            vertical_layout_fullscreen.setTimeText(time);
            vertical_layout_fullscreen.setVideoChecked(isvideo);
            vertical_layout_fullscreen.setAudioChecked(isaudio);
            vertical_layout_fullscreen.setPictureChecked(ispic);
            console.log("full isvideo = ", isvideo);
            if (true === isvideo || true === isaudio)
            {
                vertical_layout_fullscreen.setvideopath(videop);
            }
            else
            {
                vertical_layout_fullscreen.setimgCounts(picnum);
                vertical_layout_fullscreen.setimgResource(picp);
                vertical_layout_fullscreen.setPicInter(picInterval);
            }
            vertical_layout_fullscreen.getResource();
        }
        else
        {
            console.log("getAllResource in verticalImageorvideo")
            vertical_layout.setTitleText(title);
            vertical_layout.setScrollText(scrolltext);
            vertical_layout.setDateText(date);
            vertical_layout.setTimeText(time);
            vertical_layout.setVideoChecked(isvideo);
            vertical_layout.setAudioChecked(isaudio);
            vertical_layout.setPictureChecked(ispic);
            console.log("isvideo = ", isvideo);
            if (true === isvideo || true === isaudio)
            {
                vertical_layout.setvideopath(videop);
            }
            else
            {
                vertical_layout.setimgCounts(picnum);
                vertical_layout.setimgResource(picp);
                vertical_layout.setPicInter(picInterval);
            }
            vertical_layout.getResource();
        }
    }
}
