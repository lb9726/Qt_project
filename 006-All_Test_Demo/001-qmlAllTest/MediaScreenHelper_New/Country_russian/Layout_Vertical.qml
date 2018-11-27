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
        id: row_id
        x: 20
        y: 13 + 50
        width: /*170*/199
        height: /*86*/107
        Image {
            id: name
            width: parent.width
            height: parent.height
            source: "qrc:///image/vertical_img.png"
        }
    }

    Item {
        x: 307 + 143 - 280 + 129
        y: 13
        width: 82
        height: 24
        Text {
            id: time_text
            text: /*qsTr("00:00")*/ timeText
            color: "white"
            font.pixelSize: 30
            verticalAlignment: Text.AlignVCenter      // 水平居中
        }
    }

    Item {
        x: 307 + 143 -280 + 129
        y: 47
        width: 82
        height: 24
        Text {
            x: 1
            id: date_text
            text:  dateText
            color: "white"
            font.pixelSize: 14
            verticalAlignment: Text.AlignVCenter      // 水平居中
        }
    }

    Item {
        id: showImage
        x: 0
        y: /*194*/ 174
        width: 375
        height: 296 + 20
        visible: isPicture
        Image {
            id: image_back
            width: parent.width
            height: parent.height
            anchors.centerIn: parent
            source: /*"qrc:///image/Lighthouse.jpg"*/ imagesrc
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
        y: 174
        width: 375
        height: 296 + 20
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
                console.log("layout_vertical in russian the media is end!")
            }
            onPlaying: {
                console.log("layout_vertical in russian is playing")
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
                pauseorplay();
            }
        }
    }
    Item {
        id: scroll_item
        x: 0
        y: 490
        width: 375
        height: 39
        clip: true
        Text {
            id: scroll_text
            height: parent.height
            text:  scrollText
            color: "white"
            font.family: "KONE Information_v12"
            font.pixelSize: 20       // 文字大小
            verticalAlignment: Text.AlignVCenter      // 垂直居中
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
        console.log("svp videopath vertical = ", svp)
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
        if ("" != scrollText)
        {
            anima_scroll.restart(); //开启动画
        }
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
        if ("" != scrollText)
        {
            anima_scroll.restart(); //开启动画
        }
    }

    function stopVideo()
    {
        if (isVideo)
        {
            playStatus = 1;
        }

        else if (isAudio)
        {
            playStatus = 0;
        }

        player.stop();
        cycleImage.stop();
        anima_scroll.stop();
        console.log("vertical stopvideo")
    }

    function stopAudio()
    {
        if (isAudio)
        {
            console.log("player playbackState in russian = ", player.playbackState)
            if (player.StoppedState !== player.playbackState)
            {
                player.stop();
                console.log("receive signal stop audio in vertical in russian")
            }
        }
        else if (isVideo)
        {
            console.log("player playbackState in russian = ", player.playbackState)
            if (player.StoppedState !== player.playbackState)
            {
                player.stop();
                console.log("receive signal stop video in vertical in russian")
            }
        }
    }

    function pauseorplay()
    {
        console.log("Vertical playStatus in russian = ", playStatus);
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
                console.log("Vertical is call pause in russian");
            }
            else
            {
                playStatus = 1;
                player.play();
                console.log("Vertical is call play in russian");
            }
        }
    }

    function autoPlay_Audio()
    {
        if (isAudio)
        {
            if (player.PlayingState != player.playbackState)
            {
                player.play();
            }

            console.log("functiong in autoplay in layout_vertical in russian languange")
        }
        else if (isVideo)
        {
            if (player.PlayingState != player.playbackState)
            {
                player.play();
            }

            console.log("video functiong in autoplay in layout_vertical in russian languange")
        }
    }

    function clearAllParament()
    {
        isAudio = false;
        isVideo = false;
        isPicture  = false;
        videoPath = "";
        console.log("vertical in russian is clearpara")
    }

    function countScrollLengthAndRestart()
    {
        setScrollText(parameterView1.scrollString());
        scrollend = scroll_text.width;
        if ("" !== scrollText)
        {
            anima_scroll.restart();
            console.log("vertical in russian scroll_text.width and restart = ", scrollend ) ;
        }
        else
        {
            anima_scroll.stop();
            console.log("scroll is empty vertical in russian will stop");
        }
        console.log("vertical in russian in countScrollLengthAndRestart()")
    }

    Connections {
        target: MediaScreen
        onClearAudioParament: {
            if (3 !== lan_index)
            {
                console.log("vertical in russian lan_index = ", lan_index);
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
        onVerticalAudioAutoPlay: {
            autoPlay_Audio();
            console.log("vertical in russian audio will play")
            countScrollLengthAndRestart();
            console.log("vertical in russian scroll_text.width = ", scrollend ) ;
        }
    }

    Connections {
        target: ConfigureSerialer
        onStopAllAudioPlay: {
            stopAudio();
            console.log("stopAudio is call in vertical in russian")
        }
    }

    Connections {
        target: MediaScreen
        onGetScrollTextLengthSignal: {
            countScrollLengthAndRestart();
            console.log("getsignal Vertical in russian scroll_text.width = ", scrollend ) ;
        }
    }

    Component.onCompleted: {
        console.log("layout_vertical in russian is completed")
    }
}
