import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtMultimedia 5.7
import QtQuick.Controls.Material 2.0

ApplicationWindow {
    id: id_mediaScreenHelper
    visible: true
    objectName: "russianScreen"
    property alias parameterView1: area1_briVolume    //將Id在main.qml中当做全局变量使用
    property alias parameterView2: area2_timeandcycle
    property alias multiMediaView: are3_multiUpdate
    property alias resetAndMaking: are5_MakeDisk
    property int windowsHeight: 740
    property int windowsWidth: 1660

    property bool resetEnabled: false  //用户是否使能了Reset功能
    property int area1_width: 460
    property int area2_width: 680
    property int area3_width: 470
    property int area1_height: 520
    property int area4_height: 144

    property alias informationWarning: id_informationWindow
    property alias informationContent: id_informationContent

    maximumHeight:windowsHeight
    maximumWidth:windowsWidth

    minimumHeight:windowsHeight
    minimumWidth:windowsWidth
    Material.theme: Material.Light
    Material.accent: Material.Green
    title: qsTr("MediaScreen Content management") + qsTr("  V2.2.0")

    Rectangle {
        objectName: "parameterSetting"
        width: parent.width
        height: 30
        color: "transparent"
        Label {
            id: title_label
//            text: qsTr("参数设置")
            text: qsTr("Настройка параметров")
            color: "steelblue"
            font.bold: true
            font.pixelSize: 20
        }
    }
    Rectangle {
        y: 30
        color: "steelblue"
        width: parent.width
        height: 3
    }

    ColumnLayout {
        RowLayout {

        }
        RowLayout {
            Item {
                x: 17
                y: 45
                ColumnLayout {
                    RowLayout {
                        Grid {
                            rows:1
                            columns: 3
                            spacing: 5
                            Rectangle {
                                width: area1_width
                                height: area1_height
                                color: "transparent"
                                border.color: "Grey"
                                border.width: 1
                                TestRussianArea1 {
                                    id: area1_briVolume
                                    x: 10
                                    y: 10
                                }
                            }
                            Rectangle {
                                width: area2_width
                                height: area1_height
                                color: "transparent"
                                border.color: "Grey"
                                border.width: 1
                                TestRussianArea2 {
                                    id: area2_timeandcycle
                                    x: 10
                                    y: 10
                                }
                            }
                            Rectangle {
                                width: area3_width
                                height: area1_height
                                color: "transparent"
                                border.color: "Grey"
                                border.width: 1
                                TestRussianArea3 {
                                    id: are3_multiUpdate
                                    width: parent.width
                                    height: parent.height
                                    x: 10
                                    y: 10
                                    clip: true
                                }
                            }
                        }
                    }

                    RowLayout {
                        Grid {
                            rows: 1
                            columns: 2
                            spacing: 5
                            Rectangle {
                                width: area1_width
                                height: area4_height
                                color: "transparent"
                                border.color: "Grey"
                                border.width: 1
                                TestRussianArea4 {
                                    id: are4_languageChanges
                                    x: 10
                                    y: 10
                                }
                            }
                            Rectangle {
                                width: area2_width + area3_width + 5
                                height: area4_height
                                color: "transparent"
                                border.color: "Grey"
                                border.width: 1
                                TestRussianArea5 {
                                    id: are5_MakeDisk
                                    x: 10
                                    y: 10
                                    width: parent.width
                                    height: parent.height
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    Popup {
        id: id_resetWarnningBox
        x:(id_mediaScreenHelper.width - width)/2
        y:(id_mediaScreenHelper.height - height)/2
        width: 600
        height: 300
        modal: true
        focus: true
        closePolicy: Popup.NoAutoClose
        Item {
            width: 600
            height: 300
            anchors.centerIn: parent
            Text {
                id: id_resetContent
                width: parent.width
                height: parent.height
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
//                text: qsTr("打开此开关选项,屏幕将恢复默认的参数配置.多媒体,滚动字幕,标题,时间和日期,系统配置参数都将恢复为默认.")
                text: qsTr("Включите этот параметр переключателя, на экране будет восстановлена настройка параметров по умолчанию. Мультимедиа, прокрутка субтитров, заголовок, времени и даты, параметры конфигурации системы буду
т восстановлены по умолчанию.")
                wrapMode:Text.Wrap
            }
            RowLayout{
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
                height: 50
                spacing: 100
                Item {
                    width: parent.width
                    ButtonSelfDefine{
                        id: main_btn_OK
    //                    text:qsTr("确认")
                        x: 50
                        btext:qsTr("Уточнение")
                        bwidth: 140
                        bheight: 36
                        onButtonClick: {
                            id_resetWarnningBox.close();
                        }
                    }
                    ButtonSelfDefine{
                        id: main_btn_Cancel
    //                    text:qsTr("取消")
                        x: parent.width - 50 - bwidth
                        btext:qsTr("Отмена")
                        bwidth: 140
                        bheight: 36
                        onButtonClick: {
                            ConfigureSerialer.sendCanceledSignal();
                            id_resetWarnningBox.close();
                        }
                    }

                }
            }
        }
    }
    Popup {
        id: id_informationWindow
        x:(id_mediaScreenHelper.width - width)/2
        y:(id_mediaScreenHelper.height - height)/2
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
                anchors.centerIn: parent
                wrapMode:Text.Wrap
            }
            RowLayout {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
                height: 50
                spacing: 50
                Item {
                    width: parent.width
                    ButtonSelfDefine {
                        id: btn_confirm_OK
    //                    text: qsTr("确定")
                        anchors.centerIn: parent
                        btext: qsTr("Уточнение")
                        bwidth: 140
                        bheight: 36
                        onButtonClick: {
                            id_informationWindow.close();
                            ConfigureSerialer.sendPreviewCheckedFalse();
                        }
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
        x: (id_mediaScreenHelper.width - width)/2
        y: (id_mediaScreenHelper.height - height)/2
        width: 600
        height: 300
        modal: true
        closePolicy: Popup.NoAutoClose
        focus: true
        ColumnLayout {
            spacing: 50
            width: 400
            height: 200
            anchors.centerIn: parent
            ProgressBar{
                id: id_copyProgressBar
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter
                from: 0
                to: 100
                value: fileProgress
                visible: showProgress
            }
            Text{
                width: 100
                horizontalAlignment: Text.AlignHCenter
                anchors.horizontalCenter: parent.horizontalCenter
                text: id_copyProgressBar.value +"%"
                visible: showProgress
            }

            Text {
                id: id_progressContent
                anchors.horizontalCenter: parent.horizontalCenter
                text: progressContent+"\n\n" + otherInfoContent
                horizontalAlignment: Text.AlignHCenter
                wrapMode:Text.WrapAnywhere
            }

            Item {
                width: parent.width
                ButtonSelfDefine {
                    id: btn_EndMakeDisk
    //                text: qsTr("点击以结束制作")
                    anchors.centerIn: parent
                    btext: qsTr("Нажмите, чтобы закончить поделку")
                    bwidth: 464
                    bheight: 36
                    visible: showFinishedButton
                    anchors.horizontalCenter: parent.horizontalCenter
                    onButtonClick: {
                        makeDiskFinished()
                    }
                }
            }
        }
    }
    property int countDownTimes: 0
    Timer {
        id:id_copyTimer
        interval:1000
        repeat: true
        onTriggered: {
            countDownTimes ++;
            if(countDownTimes >= 6)
            {
                otherInfoContent = ""
                id_copyTimer.stop();
                countDownTimes = 0;
                if(multiMediaView.multimediaSwitcher)
                {
//                    progressContent = qsTr("拷贝文件...")
                    progressContent = qsTr("Копирование файла...")
                    if(multiMediaView.choseVideo)
                    {
                        showProgress = true;
                    }
                    else
                    {
                        showProgress = false;
                    }
                    are5_MakeDisk.area5_orderfile();
                }
                makeDiskStage2();
            }else{
                otherInfoContent = "( " + (6 -countDownTimes)+" )";
            }
        }
    }

    Connections {
        target: ConfigureSerialer
        onResetDefaultClicked: {
            id_resetWarnningBox.open();

        }
    }

    property bool enterFlag: false
    Connections {
        target: ConfigureSerialer
        onCopyFileProgress: {
            fileProgress = pro;
            if(pro == 100 && enterFlag == false)
            {
                console.log("enterFlag= ", enterFlag)
                enterFlag = true;
                fileProgress = 0;
                makeDiskStage2();
            }
        }
    }

    Popup {
        id: id_ColseWarnningBox
        x:(id_mediaScreenHelper.width - width)/2
        y:(id_mediaScreenHelper.height - height)/2
        width: 600
        height: 300
        modal: true
        focus: true
        closePolicy: Popup.NoAutoClose
        Item {
            width: 600
            height: 300
            anchors.centerIn: parent
            Text {
                id: id_ColseContent
                width: parent.width
                height: parent.height

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
//                text: qsTr("点击确认，将重新加载界面布局，Usb设备需要重新插拔！.")
                text: qsTr("Нажмите, чтобы подтвердить.Размещение страницы будет перезагружен.Необходимо перезагрузить USB!")
                wrapMode:Text.Wrap
            }
            RowLayout{
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
                height: 50
                spacing: 50
                Item {
                    width: parent.width
                    ButtonSelfDefine {
                        id: mainColse_btn_OK
    //                    text: qsTr("确认")
                        x: 50
                        btext: qsTr("Уточнение")
                        bwidth: 140
                        bheight: 36
                        onButtonClick: {
                            id_resetWarnningBox.close();
                            id_mediaScreenHelper.close();
                            MediaScreen.changeUi(); // 切换非俄语布局界面
                            MediaScreen.sendUdiskInformation(); // 切换界面显示Usb
                        }
                    }
                    ButtonSelfDefine{
                        id: mainColse_btn_Cancel
    //                    text: qsTr("取消")
                        x: parent.width - 50 - bwidth
                        btext:qsTr("Отмена")
                        bwidth: 140
                        bheight: 36
                        onButtonClick: {
                            MediaScreen.setIndex(3);
                            are4_languageChanges.ischeckedchange();
                            id_ColseWarnningBox.close();
                        }
                    }
                }
            }
        }
    }

    Vertical_ImageOrVideo {
        id: open_vertical_view
    }

    Horizontal_ImageOrVideo {
        id: open_horizontal_view
    }

    function openChangeLayout()
    {
        id_ColseWarnningBox.open();
    }

    function makeDiskStage2() {
        showProgress = false;
//        progressContent = qsTr("正在生成配置参数");
        progressContent = qsTr("Создание параметров конфигурации");
        ConfigureSerialer.createXml(are5_MakeDisk.are5_DiskPath());
//        progressContent = qsTr("升级盘制作成功");
        progressContent = qsTr("Успех в поделке обновлении диска");
        showFinishedButton = true;
    }

    function makeDiskFinished(){
        progressContent = ""
        progressON = false;
        showProgress = false;
        showFinishedButton = false;
        enterFlag = false;
    }

    function format_text()
    {
//        progressContent = qsTr("正在格式化U盘...")
        progressContent = qsTr("проверить диска...")
    }

    function setprogressOn()
    {
        progressON = true;
    }
    function donotFormatUdisk()
    {
        console.log("donotFormatUdisk()")
        if(multiMediaView.multimediaSwitcher)
        {
            progressContent = qsTr("拷贝文件...")
            if(multiMediaView.choseVideo || multiMediaView.choseAudio)
            {
                showProgress = true;
                console.log("donotFormatUdisk() showProgress")
            }
            else
            {
                showProgress = false;
                console.log("donotFormatUdisk() not showProgress")
            }
            are5_MakeDisk.area5_orderfile();
        }
        makeDiskStage2();
    }
    Component.onCompleted: {
        UsbHelper.scanHD();
    }
}
