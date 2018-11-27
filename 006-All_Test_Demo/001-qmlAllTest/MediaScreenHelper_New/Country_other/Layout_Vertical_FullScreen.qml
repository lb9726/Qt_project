import QtQuick 2.0
import QtMultimedia 5.5

Rectangle {
    property string titleText: ""
    property string timeText: ""
    property string dateText: ""
    property string scrollText: ""
    property string imagesrc: ""
    property int scrollend: 0
    property int playStatus: 1
    property int imagesCount: 0
    property bool isVideo: false
    property bool isAudio: false
    property bool isPicture: false
    property string videoPath: ""
    property var picList: ""
    property int curImageindex: 0
    property int picIntertime: 3000

    width: 375
    height: 530
    color: "black"

    Item {
        x: 20
        y: 13
        width: 263
        height: 29
        visible: false
        Text {
            id: title_text
            width: parent.width
            elide: Text.ElideMiddle
            text: titleText
            color: "white"
            font.pixelSize: 30
        }
    }

    Item {
        x: 307 + 143
        y: 13
        width: 82
        height: 24
        visible: false
        Text {
            id: time_text
            text: timeText
            color: "white"
            font.pixelSize: 30
            verticalAlignment: Text.AlignVCenter      // 水平居中
        }
    }

    Item {
        x: 307 + 143
        y: 47
        width: 82
        height: 24
        visible: false
        Text {
            x: 1
            id: date_text
            text: dateText
            color: "white"
            font.pixelSize: 14
            verticalAlignment: Text.AlignVCenter      // 水平居中
        }
    }

    Item {
        id: showImage
        x: 0
        y: 0
        width: parent.width
        height: parent.height
        visible: isPicture
        Image {
            id: image_back
            width: parent.width
            height: parent.height
            anchors.centerIn: parent
            source: imagesrc
        }
        Timer {
            id: cycleImage
            running: false;
            interval: picIntertime
            repeat: true;
            onTriggered: {
                changeImages();
            }
        }
    }

    Item {
        id: showVideo
        x: 0
        y: 0
        width: parent.width
        height: parent.height
        visible: (isVideo ||isAudio)

        AnimatedImage {
            visible: isAudio
            id: gif_img
            width: parent.width; height: parent.height
            source: "qrc:///image/gifimg.gif"
        }

        MediaPlayer{
            id: player
            source: videoPath
            autoLoad: false
            autoPlay: false/*(isAudio ? true:false)*/    // 当AutoPlay 设置为true的时候，如果视频文件存在，就会直接播放视频
            volume: 0.4
            loops: MediaPlayer.Infinite
            onStopped: {
                console.log("layout_vertical_fullscreen the media is end!")
            }
            onPlaying: {
                console.log("layout_vertical_fullscreen is playing")
            }
        }
        VideoOutput {
            width: parent.width; height: parent.height
            anchors.fill: parent
            source: player
            fillMode:VideoOutput.Stretch
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                pauseorplay(); console.log("videopath = ", videoPath);
            }
        }
    }
    Item {
        id: scroll_item   //x: 290 y: 684
        x: 0
        y: /*488*/ 468
        width: /*389*/527
        height: 29 + 20
        clip: true
        visible: false
        Text {
            id: scroll_text
            height:parent.height
            text:  scrollText
            color: "white"
            font.family: "KONE Information_v12"
            font.pixelSize: 20       // 文字大小
            verticalAlignment: Text.AlignVCenter      // 水平居中
            SequentialAnimation on x {
                running: false
                id: anima_scroll
                loops: Animation.Infinite
                PropertyAnimation {
                    from: scroll_item.width
                    to:  -scrollend    // 传数值时如果running为true，则导致动画重点不对
                    duration: 12000      // 滚动的文字运行的时间长度
                }
            }
        }
    }

    function setTitleText(stitext)
    {
        titleText = stitext;
        console.log("titleText =", titleText);
    }

    function setTimeText(stt)
    {
        timeText = stt;
        console.log("timeText =", timeText);
    }

    function setDateText(sdt)
    {
        dateText = sdt;
        console.log("dateText = ", dateText);
    }

    function setScrollText(sst)
    {
        scrollText =  sst;
        console.log("scrollText = ", scrollText);
    }

    function setVideoChecked(ischecked)
    {
        isVideo = ischecked;
    }

    function setAudioChecked(ischecked)
    {
        isAudio = ischecked;
    }

    function setPictureChecked(ischecked)
    {
        isPicture = ischecked;
    }

    function setvideopath(svp)
    {
        videoPath = svp;
        console.log("svp videopath verticalfullscreen = ", svp)
    }

    function setimgCounts(num)
    {
        imagesCount = num;
    }

    function setimgResource(iResource)
    {
        picList = iResource;
        console.log("piclist = ", picList);
    }

    function getimgResource()
    {
        return picList;
    }

    function setPicInter(picI)
    {
        picIntertime = picI * 1000;
    }

    function getResource()
    {
        scrollend = scroll_text.width;
        console.log("vertical scroll_text.width = ", scrollend ) ;

        if (isPicture)
        {
            if(picList.length > 1)
            {
                console.log("is large than 1 ,start ....")
                console.log("piclist.length = ", picList.length)
                cycleImage.start();
            }
            else
            {
                imagesrc = picList[0];
            }
        }
    }

    function changeImages()
    {
        if (curImageindex <= imagesCount - 1)
        {
            imagesrc = picList[curImageindex];
            curImageindex++;
        }
        else
        {
            curImageindex = 0;
            imagesrc = picList[curImageindex++];
        }
    }

    function startAnimation()
    {
        if (imagesCount > 1)
        {
            cycleImage.start();
        }
    }

    function stopVideo()
    {
        if (isVideo)
            playStatus = 1;
        else if (isAudio)
            playStatus = 0;
        player.stop();
        cycleImage.stop();
        anima_scroll.stop();
        console.log("vertical_fullscreen stopvideo")
    }

    function stopAudio()
    {
        if (isAudio)
        {
            console.log("player playbackState = ", player.playbackState)
            if (player.StoppedState !== player.playbackState)
            {
                player.stop();
                console.log("receive signal stop audio in vertical fullscreen")
            }
        }
        else if (isVideo)
        {
            console.log("player playbackState = ", player.playbackState)
            if (player.StoppedState !== player.playbackState)
            {
                player.stop();
                console.log("receive signal stop video in vertical fullscreen")
            }
        }
    }

    function pauseorplay()
    {
        console.log("Vertical playStatus = ", playStatus);
        if (isVideo)
        {
            if (0 === playStatus)
            {
                playStatus = 1;
                player.play();
            }
            else
            {
                playStatus = 0;
                player.pause();
            }
        }
        else if (isAudio)
        {
            if (1 === playStatus)
            {
                playStatus = 0;
                player.pause();
                console.log("Vertical fullscreen is call pause");
            }
            else
            {
                playStatus = 1;
                player.play();
                console.log("Vertical fullscreen is call play");
            }
        }
    }

    function autoPlay_Audio()
    {
        if (isAudio)
        {
            if (player.PlayingState !== player.playbackState)
            {
                player.play();
            }

            console.log("functiong in autoplay in layout_vertical_fullscreen in normal languange")
        }
        else if (isVideo)
        {
            if (player.PlayingState !== player.playbackState)
            {
                player.play();
            }

            console.log("video functiong in autoplay in layout_vertical_fullscreen in normal languange")
        }
    }

    function clearAllParament()
    {
        isAudio = false;
        isVideo = false;
        isPicture  = false;
        videoPath = "";
        console.log("vertical fullscreen in normal is clearpara")
    }

    Connections {
        target: MediaScreen
        onClearAudioParament: {
            if (3 === lan_index)
            {
                console.log("vertical fullscreen in normal is clearPara lan_index = ", lan_index);
                clearAllParament();
            }
        }
    }

    Connections {
        target: ConfigureSerialer
        onStartPicAnimation: {
            startAnimation();
        }
    }

    Connections {
        target: ConfigureSerialer
        onVerticalFullscreenAudioAutoPlay: {
            autoPlay_Audio();
            console.log("vertical_fullscreen audio will play")
        }
    }

    Connections {
        target: ConfigureSerialer
        onStopAllAudioPlay: {
            stopAudio();
            console.log("stopAudio is call in vertical fullscreen")
        }
    }

    Component.onCompleted: {
        console.log("layout_vertical_fullscreen is completed")
    }
}
