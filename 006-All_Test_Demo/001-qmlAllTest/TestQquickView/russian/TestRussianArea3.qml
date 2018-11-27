import QtQuick 2.7
import QtQuick.Layouts 1.0
import QtQuick.Controls.Styles 1.3
import QtQuick.Controls 1.3
import QtQuick.Dialogs 1.2

Item {
    property int area3_width: 310
    property int area1_height: 560
    property string videoPath: "" //本地视频文件路径
    property string audioPath: "" //本地音频文件路径
    property bool multimediaSwitcher: (video_text_btn.checked || picture_text_btn.checked || audio_text_btn.checked)
    property alias choseVideo: video_text_btn.checked  //true —— 视频，false —— 图片
    property alias choseAudio: audio_text_btn.checked
    property alias chosePicture: picture_text_btn.checked
    property alias isFullScreen: fullscreen_show.checked  //true —— 全屏显示，false —— 非全屏显示
    property variant fullScreenStrList: [12] ;
    width: area3_width
    height: area1_height

    Text {
        id: updateMedia_id
        x: 0
        y: 0
        // text: qsTr("多媒体更新")
        text: qsTr("Обновление мультимедиа")
        //        font.pixelSize: 12
    }

    RadioButton {
        id: normal_show
        ExclusiveGroup { id: showGroup }
        exclusiveGroup: showGroup
        checked: true
        x: 50
        y: 28
        //text: qsTr("正常显示")
        text: qsTr("Нормальная индикация")
    }

    RadioButton {
        id: fullscreen_show
        exclusiveGroup: showGroup
        x: 50
        y: 55
        text: qsTr("Полноэкранная индикация") // text: qsTr("全屏显示")
        onCheckedChanged: {
            console.log("cheched fullscreen is = ", checked);
            ConfigureSerialer.sendIsFullScreenSignal(checked);
            if (checked)
            {
                if (checkFullScreenIsRight("checked"))
                {
                    ConfigureSerialer.sendCheckFullScreenSignal(checked);
                }
            }
        }
    }

    property int videoFlag: 0
    property int audioFlag: 0
    property int pictureFlag: 0

    Text {
        id: media_content
        x: 0
        y: 85
        width: 60
        height: 12
        text: qsTr("Добавить мультимедиа") // 添加多媒体
        //        font.pixelSize: 12
    }

    RadioButton {
        id: video_text_btn
        ExclusiveGroup { id: selectGroup }
        exclusiveGroup: selectGroup
        x: 31 - 11 + 20
        y: 120
        width: 62
        height: 16
        //text: qsTr("视频")
        text: qsTr("Видео")
        onCheckedChanged: {
            multimediaSwitcher = (video_text_btn.checked || picture_text_btn.checked ||audio_text_btn.checked);
            console.log("video checked = ", video_text_btn.checked);
            console.log("video multimediaSwitcher = ", multimediaSwitcher);
        }
    }

    RadioButton {
        id: audio_text_btn
        exclusiveGroup: selectGroup
        x: 116 + 36 + 20
        y: 120
        width: 55
        height: 16
        //text: qsTr("音频")
        text: qsTr("аудио")
        onCheckedChanged: {
            multimediaSwitcher = (video_text_btn.checked || picture_text_btn.checked ||audio_text_btn.checked);
            console.log("audio checked = ", audio_text_btn.checked);
            console.log("audio multimediaSwitcher = ", multimediaSwitcher);
        }
    }

    RadioButton {
        id: picture_text_btn
        exclusiveGroup: selectGroup
        x: 198 + 86 + 20
        y: 120
        width: 45
        height: 16
        //text: qsTr("图片")
        text: qsTr("Картина")
        onCheckedChanged: {
            multimediaSwitcher = (video_text_btn.checked || picture_text_btn.checked ||audio_text_btn.checked);
            console.log("picture checked = ", picture_text_btn.checked);
            console.log("picture multimediaSwitcher = ", multimediaSwitcher);
        }
    }

    Item {
        id: select_pictureOrvideo
        width: parent.width
        height: 100
        visible: video_text_btn.checked
        x: 0
        y: 230
        Item {
            width: parent.width - 16
            height: id_videoPathLabel.height
            x: 0
            y: 0
            Text {
                id: id_videoPathLabel
                visible: text.length > 0
                text: videoPath
                width: parent.width
                elide: Text.ElideMiddle
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
        ButtonSelfDefine {
            x: 0
            y: 20
            id: select_btn_video
            anchors.centerIn: parent
            // text: qsTr("选择视频")
            btext: qsTr("Выбор видео")
            bwidth: 195
            bheight: 36
            onButtonClick: {
                addVideo();
            }
        }
    }
    Item {
        id: id_audioArea
        width: parent.width
        height: 100
        visible: audio_text_btn.checked
        x: 0
        y: 230
        Item {
            width: parent.width - 16
            height: id_audioPathLabel.height
            x: 0
            y: 0
            Text {
                id: id_audioPathLabel
                visible: text.length > 0
                text: audioPath
                width: parent.width
                elide: Text.ElideMiddle
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
        ButtonSelfDefine {
            x: 0
            y: 20
            id: select_btn_audio
            anchors.centerIn: parent
            btext: qsTr("выбрать аудио") // 选择音频
            bwidth: 195
            bheight: 36
            onButtonClick: {
                addAudio();
            }
        }
    }

    Item {
        id: id_pictureArea
        visible: picture_text_btn.checked
        width: parent.width
        height: 100
        y: 200 + 30
        ButtonSelfDefine {
            id: select_picture_btn
            //text: qsTr("选择图片")
            btext: qsTr("Выбор картины")
            bwidth: 195
            bheight: 36
            anchors.centerIn: parent
            visible: id_pickedPictureModel.count === 0
            onButtonClick: {
                addPictures();
            }
        }
    }

    Item {
        id: picture_rectangle
        width: parent.width
        height: 230
        x: 0
        y: 170
        RowLayout {
            visible: (id_pickedPictureModel.count !== 0 && picture_text_btn.checked)
            height: 32
            width: parent.width
            Item {
                anchors.fill: parent
                RowLayout {
                    anchors.centerIn: parent
                    spacing: 15
                    Label {
                        id: interval_text
                        text: qsTr("интервал")// 间隔
                    }
                    SpinBox {
                        id: id_picture_Interval
                        minimumValue: 1
                        maximumValue : 100
                        value: 3
                        onValueChanged: {
                            console.log("id_picture_Interval.value = ", value)
                        }
                    }
                }
            }
        }

        ColumnLayout {
            anchors.centerIn: parent
            visible: (id_pickedPictureModel.count !== 0 && picture_text_btn.checked)
            Item {
                id: showPicture_list
                width: 200
                height: 200 - 20 -20
                anchors.centerIn: parent
                Rectangle {
                    color: "transparent"
                    border.width: 1
                    border.color: "gray"
                    opacity: 0.6
                    anchors.fill: parent
                }
                ListView {
                    id:id_pictureListView
                    width: parent.width
                    height: parent.height - 5
                    model: id_pickedPictureModel
                    clip: true
                    anchors.centerIn: parent
                    delegate: Rectangle{
                        height: 30
                        width:id_pictureListView.width
                        clip: true
                        color: (index === id_pictureListView.currentIndex ? "grey":"transparent")
                        Text {
                            text: path
                            width: parent.width
                            elide: Text.ElideMiddle
                            anchors.centerIn: parent
                        }
                        MouseArea {
                            anchors.fill:parent
                            onClicked: {
                                id_pictureListView.currentIndex = index;
                            }
                        }
                    }
                }
            }
            RowLayout {
                height: 40
            }
            RowLayout {
                id: btn_group
                Item {
                    ButtonSelfDefine {
                        id: btn_Add
                        // text:qsTr("添加")
                        btext:qsTr("Добавить")
                        x: 0
                        bwidth: 66
                        bheight: 36
                        onButtonClick:  {
                            addPictures();
                            var res = updateMediaSetting();
                            console.log("res = ", res);
                        }
                    }
                    ButtonSelfDefine {
                        id: btn_Delete
                        x: btn_Add.width + 5
                        // text: qsTr("删除")
                        btext: qsTr("Удалить")
                        bwidth: 66
                        bheight: 36
                        onButtonClick:
                        {
                            deletePicture(id_pictureListView.currentIndex);
                        }
                    }
                    ButtonSelfDefine {
                        id: btn_Clear
                        x: btn_Add.width + btn_Delete.width + 10
                        // text: qsTr("清空")
                        btext: qsTr("Очистить")
                        bwidth: 66
                        bheight: 36
                        onButtonClick: clearPictures();
                    }

                }
            }
        }
    }

    Item {
        x: 0
        y: parent.height - btn_screenPreview.height - 25
        height: 40
        width: parent.width
        ButtonSelfDefine {
            id: btn_screenPreview
            x: 0
            anchors.centerIn: parent
            btext: qsTr("интерфейс просмотр")//界面预览Screen preview
            bwidth: 276
            bheight: 36
            onButtonClick: {
                console.log("checkIsShowPreview() is called");
                checkIsShowPreview();
                MediaScreen.getScrollTextLength();
                ConfigureSerialer.sendPlayAfterClose(multiMediaView.isCheckedFullScreen());
            }
        }
//        Button {
//            id: btn_screenPreview
//            x: 0
//            anchors.centerIn: parent
//            text: qsTr("интерфейс просмотр")//界面预览Screen preview
//            onClicked: {
//                console.log("checkIsShowPreview() is called");
//                checkIsShowPreview();
//                MediaScreen.getScrollTextLength();
//                ConfigureSerialer.sendPlayAfterClose(multiMediaView.isCheckedFullScreen());
//            }
//            style: ButtonStyle {
//                background: BorderImage {
//                    source: btn_screenPreview.pressed ? "qrc:///image/Brown_276_40.png":"qrc:///image/Blue_276_40.png"
//                    width: btn_screenPreview.width; height: btn_screenPreview.height
//                    border.left: 5; border.top: 5
//                    border.right: 5; border.bottom: 5
//                }
//                label:Rectangle {
//                    color: "transparent"
//                    Text{
//                        id: btn_screenPreview_text
//                        text: btn_screenPreview.text
//                        color:"white"
//                        anchors.centerIn: parent
//                        font.family: "KONE Information_v12"
//                    }
//                }
//            }
//        }
    }

    function updateMediaSetting()
    {
        console.log("will copy videopath  in russian is", videoPath );
        if(!multimediaSwitcher)
        {
            console.log("all is not select in russian")
            ConfigureSerialer.updateMultiMedia(false, choseVideo, choseAudio, isFullScreen, "");
        }
        else
        {
            var res = "";
            if(choseVideo)
            {
                console.log("video is select in russian",videoPath)
                res = videoPath;
            }
            else if (choseAudio)
            {
                console.log("audio is select", audioPath)
                res = audioPath;
            }
            else
            {
                console.log("picture is select in russian")
                for(var i=0; i<id_pickedPictureModel.count; i++)
                {
                    res += id_pickedPictureModel.get(i).path + "*";
                }
                ConfigureSerialer.updatePictureInterval(id_picture_Interval.value);
            }
            console.log("will copy videopath res in russian is", res );
            ConfigureSerialer.updateMultiMedia(true, choseVideo, choseAudio, isFullScreen, res);
        }
    }

    function addVideo() {
        id_pickVideoFileDialog.open();
    }

    function addAudio()
    {
        id_pickAudioFileDialog.open();
    }

    function shiftAbsolutePath(path) {
        var pre = path.lastIndexOf("file:///");
        if(pre >= 0) {
            //console.log("shiftAbsolutePath 2"+path.substring(8));
            return path.substring(8);
        }else {
            return "";
        }
    }

    FileDialog {
        id: id_pickVideoFileDialog
        // title: qsTr("请选择一个视频文件")
        title: qsTr("Выберите видео")
        folder: shortcuts.home
        selectMultiple: false
        nameFilters: [ "video files (*.mp4 *.avi *.mkv)" ]
        onAccepted: {/*console.log(id_videoPathLabel.width,id_videoPathLabel.height)*/
            videoPath = shiftAbsolutePath(id_pickVideoFileDialog.fileUrl.toString());
        }
    }

    FileDialog {
        id: id_pickAudioFileDialog
        //title: qsTr("请选择一个音频文件")
        title: qsTr("выбрать аудио")
        folder: shortcuts.home
        selectMultiple: false
        nameFilters: [ "audio files (*.mp3 *.wav)" ]
        onAccepted: {/*console.log(id_videoPathLabel.width,id_videoPathLabel.height)*/
            audioPath = shiftAbsolutePath(id_pickAudioFileDialog.fileUrl.toString());
        }
    }
    ListModel {
        id: id_pickedPictureModel
    }

    function addPictures() {
        id_pickImageFileDialog.open();
    }

    function deletePicture(index)  {
        id_pickedPictureModel.remove(index);
    }

    function clearPictures() {
        id_pickedPictureModel.clear();
    }

    FileDialog {
        id: id_pickImageFileDialog
        //        title: qsTr("请选取图片")
        title: qsTr("Выберите картину")
        folder: shortcuts.home
        selectMultiple: true
        nameFilters: [ "Image files (*.png *.jpg *.bmp)" ]
        onAccepted: {
            for( var i = 0; i < id_pickImageFileDialog.fileUrls.length; ++i)
            {
                id_pickedPictureModel.append({"path": shiftAbsolutePath(id_pickImageFileDialog.fileUrls[i])});
            }
        }
    }

    Connections {
        target: Ctranslator
        onLanguageChanges: {
            changesLanguge();
        }
    }

    function checkMediaSetting(){
        var res = "";
        if(choseVideo){
            if(videoPath === ""){
                //                return qsTr("请选择一个视频")
                return qsTr("Выберите видео")
            }
        }
        if (choseAudio)
        {
            if(audioPath === ""){
                //                return qsTr("请选择一个音频")
                return qsTr("выбрать аудио")
            }
        }

        if (chosePicture) {
            if(id_pickedPictureModel.count == 0){
                //                return qsTr("请选择图片")
                return qsTr("Выберите картину")
            }
        }
        return "";
    }

    function changesLanguge()
    {
        console.log("语言改变 Area3")
        updateMedia_id.text = qsTr("多媒体更新")
        normal_show.text = qsTr("正常显示")
        fullscreen_show.text = qsTr("全屏显示")
        video_text_btn.text = qsTr("视频")
        audio_text_btn.text = qsTr("音频")
        picture_text_btn.text = qsTr("图片")
        select_btn_video.text = qsTr("选择视频")
        select_picture_btn.text = qsTr("选择图片")
        select_btn_audio.text = qsTr("选择音频")
        btn_Add.text = qsTr("添加")
        btn_Delete.text = qsTr("删除")
        btn_Clear.text = qsTr("清空")
        id_pickVideoFileDialog.title = qsTr("请选择一个视频文件")
        id_pickImageFileDialog.title = qsTr("请选取图片")
        interval_text.text = qsTr("间隔")
        media_content.text = qsTr("添加多媒体");
        btn_screenPreview.text = qsTr("界面预览")
        //        pictureSelectPath();
    }

    function videoIsCheck()
    {
        return choseVideo
    }

    function audioIsCheck()
    {
        return choseAudio
    }

    function pictureIsCheck()
    {
        return chosePicture;
    }

    function videoSelectPath() // 返回视频的路径供外部使用
    {
        var src = "";
        if (choseVideo)
            src = "file:///" + videoPath;
        else if (choseAudio)
        {
            src = "file:///" + audioPath;
        }
        return src;
    }

    function pictureCounts()
    {
        var num = 0;
        for(var i = 0; i < id_pickedPictureModel.count; i++)
        {

        }
        num = i;
        return num;
    }

    function pictureSelectPath() // 返回图片的路径供外部使用
    {
        console.log("pictureSelectPath is called")
        var res = "";
        var res1 = "";
        if (chosePicture)
        {
            for(var i = 0; i<id_pickedPictureModel.count; i++)
            {
                res += id_pickedPictureModel.get(i).path + "*";
            }
            res1 = ConfigureSerialer.retPicturePath(res);
            for(var j = 0; j < i; j++)
            {
                //                console.log("res = ", res1[j]);
                res1[j] = "file:///" + res1[j];
            }
        }
        //        console.log("res1 = ", res1);
        return res1;
    }

    function cycleSeconds()
    {
        return id_picture_Interval.value;
    }

    function getIsfullScreen()
    {
        return fullscreen_show.checked;
    }

    function setVideoBtnStatus()
    {
        if (0 === videoFlag)
        {
            if (0 === audioFlag && 0 === pictureFlag)
            {
                video_text_btn.checked = true;
                videoFlag = 1;
                console.log("others all false")
            }
            else if (1 === audioFlag)
            {
                video_text_btn.checked = true;
                videoFlag = 1;
                audio_text_btn.checked = false;
                audioFlag = 0;
                console.log("audio is ture will set false")
            }
            else if (1 === pictureFlag)
            {
                video_text_btn.checked = true;
                videoFlag = 1;
                picture_text_btn.checked = false;
                pictureFlag = 0;
                console.log("picutre is ture will set false")
            }
        }
        else if (1 === videoFlag)
        {
            video_text_btn.checked = false;
            videoFlag = 0;
            console.log("videoFlag will true->false")
        }
    }

    function setAudioBtnStatus()
    {
        if (0 === audioFlag)
        {
            if (0 === videoFlag && 0 === pictureFlag)
            {
                audio_text_btn.checked = true;
                audioFlag = 1;
                console.log("others all false")
            }
            else if (1 === videoFlag)
            {
                audio_text_btn.checked = true;
                audioFlag = 1;
                video_text_btn.checked = false;
                videoFlag = 0;
                console.log("video is ture will set false")
            }
            else if (1 === pictureFlag)
            {
                audio_text_btn.checked = true;
                audioFlag = 1;
                picture_text_btn.checked = false;
                pictureFlag = 0;
                console.log("picutre is ture will set false")
            }
        }
        else if (1 === audioFlag)
        {
            audio_text_btn.checked = false;
            audioFlag = 0;
            console.log("audioFlag will true->false")
        }
    }

    function setPictureBtnStatus()
    {
        if (0 === pictureFlag)
        {
            if (0 === videoFlag && 0 === audioFlag )
            {
                picture_text_btn.checked = true;
                pictureFlag = 1;
                console.log("others all false")
            }
            else if (1 === videoFlag)
            {
                picture_text_btn.checked = true;
                pictureFlag = 1;
                video_text_btn.checked = false;
                videoFlag = 0;
                console.log("video is ture will set false")
            }
            else if (1 === audioFlag)
            {
                picture_text_btn.checked = true;
                pictureFlag = 1;
                audio_text_btn.checked = false;
                audioFlag = 0;
                console.log("audio is ture will set false")
            }
        }
        else if (1 === pictureFlag)
        {
            picture_text_btn.checked = false;
            pictureFlag = 0;
            console.log("pictureFlag will true->false")
        }
    }

    function isCheckedFullScreen()
    {
        if (isFullScreen)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    function checkIsShowPreview()
    {
        var ret;
        //        ret = parameterView1.checkParameterSetting();
        //        console.log("before ret = ", ret);
        //        if(ret !== "" && !informationWarning.visible){
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
        if(ret !== "" && !informationWarning.visible){
            console.log("area3 checked!");
            informationContent.text = ret;
            if (resetAndMaking.getResetDefault())
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

    function setEnableFlag(resetFlag)
    {
        if (resetFlag)
        {
            normal_show.enabled = false;
            fullscreen_show.enabled = false;
            video_text_btn.enabled = false;
            audio_text_btn.enabled = false;
            picture_text_btn.enabled = false;
            select_pictureOrvideo.enabled = false;
            id_audioArea.enabled = false;
            id_pictureArea.enabled = false;
            picture_rectangle.enabled = false;
            showPicture_list.enabled = false;
            btn_group.enabled = false;
        }
        else
        {
            normal_show.enabled = true;
            fullscreen_show.enabled = true;
            video_text_btn.enabled = true;
            audio_text_btn.enabled = true;
            picture_text_btn.enabled = true;
            select_pictureOrvideo.enabled = true;
            id_audioArea.enabled = true;
            id_pictureArea.enabled = true;
            picture_rectangle.enabled = true;
            showPicture_list.enabled = true;
            btn_group.enabled = true;
        }
    }
    function checkFullScreenIsRight(para)
    {
        if (fullScreenStrList[1] === para)
        {
            return false;
        }
        else if (fullScreenStrList[2] === para)
        {
            return true;
        }
    }

    function fullScreenCancel()
    {
        fullscreen_show.checked = false;
        normal_show.checked = true;
    }
}
