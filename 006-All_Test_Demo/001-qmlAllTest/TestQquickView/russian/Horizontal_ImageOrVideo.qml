import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls.Material 2.0

Popup {
    property string vborder_Color: "red";
    property string hborder_Color: "grey"
    property bool isFullScreen: false;
    id: id_resetWarnningBox
    x:(id_mediaScreenHelper.width - width)/2
    y:(id_mediaScreenHelper.height - height)/2
    width: 600
    height: 545 + 20
    modal: true
    focus: true
    closePolicy: Popup.NoAutoClose
    Button_Circle {
        x: parent.width - 20
        y: -5
        onButtonclick: {
            horizon_layout.stopVideo();
            horizon_layout_fullscreen.stopVideo();
            open_horizontal_view.close();
//            ConfigureSerialer.sendPreviewCheckedFalse();
            ConfigureSerialer.sendStopAudioPlay();
        }
    }

    Item {
        x: 0
        y: 0
        width: 96
        height: 46
        Text {
            x: 0
            y: -5
            id: layout_preview
            text: qsTr("интерфейс просмотр") //  界面预览
//            anchors.centerIn: parent
        }
    }
    Rectangle {
        id: greyline
        x: /*13*/2
        y: 46
        width: 585
        height: 2
        color: "grey"
    }

    Rectangle {
        x: 42 - 16 -18
        y: 65
        id: hRect
        width: 280 /*btn_horizatal.width + 4*/
        height: btn_horizatal.height + 4
//        border.color: hborder_Color
//        border.width: 1
        visible: !isFullScreen
        ButtonSelfDefine {
            id: btn_horizatal
            anchors.centerIn: parent
            checkable: true
            enabled: false
            btext: qsTr("кросс экраном")  // 横屏显示
            bwidth: 276
            bheight: 36
            onButtonClick :
            {
                open_vertical_view.close();
                open_horizontal_view.open();
            }
        }

//        Button_Self {
//            id: btn_horizatal
//            anchors.centerIn: parent
//            checkable: true
//            enabled: false
//            text: qsTr("кросс экраном")  // 横屏显示
//            onClicked:
//            {
//                open_vertical_view.close();
//                open_horizontal_view.open();
//            }
//        }
    }
    Rectangle {
        x: 310 - 18
        y: 65
        id: vRect
        width: 280 /*btn_vertical.width + 4*/
        height: btn_vertical.height + 4
//        border.color: vborder_Color
//        border.width: 1
        visible: !isFullScreen
        Button_Self_Inactive {
            id: btn_vertical
            anchors.centerIn: parent
            checkable: true;
            checked: true;
            btext: qsTr("вертикальный экран") // 竖屏显示
            bwidth: 280
            bheight: 36
            onButtonClick:
            {
                horizon_layout.stopVideo();
                horizon_layout_fullscreen.stopVideo();
                open_horizontal_view.close();                
                open_vertical_view.open();
                ConfigureSerialer.sendVerticalAutoPlay(isFullScreen);
                ConfigureSerialer.sendStartPicAnimation();
            }
        }
    }
    Layout_Horizontal {
        id: horizon_layout
        x: 38 - 15
        y: 154 - 20
//        width: 530
//        height: 375
        visible: !isFullScreen
    }

    Layout_Horizontal_FullScreen
    {
//        width:  600
//        height: 414
        x: 38 - 15
        y: 154 - 20 - 45
        id: horizon_layout_fullscreen
        visible: isFullScreen
    }

    Connections {
        target: ConfigureSerialer
        onShowPreviewSignal: {
            open_horizontal_view.open();
        }
    }

    Connections {
        target: ConfigureSerialer
        onIsFullScreenChanges: {
            isFullScreen = isfull;
        }
    }

    function getAllResource(title, scrolltext, date, time, isvideo, isaudio, ispic, videop, picnum, picp, picInterval, isfull)
    {
        if (true === isfull)
        {
            console.log("getAllResource in HorizontalImageorvideo")
            horizon_layout_fullscreen.setTitleText(title);
            horizon_layout_fullscreen.setScrollText(scrolltext);
            horizon_layout_fullscreen.setDateText(date);
            horizon_layout_fullscreen.setTimeText(time);
            horizon_layout_fullscreen.setVideoChecked(isvideo);
            horizon_layout_fullscreen.setAudioChecked(isaudio);
            horizon_layout_fullscreen.setPictureChecked(ispic);
            if (true === isvideo || true === isaudio)
            {
                horizon_layout_fullscreen.setvideopath(videop);
            }
            else
            {
                horizon_layout_fullscreen.setimgCounts(picnum);
                horizon_layout_fullscreen.setimgResource(picp);
                horizon_layout_fullscreen.setPicInter(picInterval);
            }
            horizon_layout_fullscreen.getResource();
        }
        else
        {
            console.log("getAllResource in HorizontalImageorvideo")
            horizon_layout.setTitleText(title);
            horizon_layout.setScrollText(scrolltext);
            horizon_layout.setDateText(date);
            horizon_layout.setTimeText(time);
            horizon_layout.setVideoChecked(isvideo);
            horizon_layout.setAudioChecked(isaudio);
            horizon_layout.setPictureChecked(ispic);
            if (true === isvideo || true === isaudio)
            {
                horizon_layout.setvideopath(videop);
            }
            else
            {
                horizon_layout.setimgCounts(picnum);
                horizon_layout.setimgResource(picp);
                horizon_layout.setPicInter(picInterval);
            }
            horizon_layout.getResource();
        }
    }
}
