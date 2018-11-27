import QtQuick 2.0
import QtMultimedia 5.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.0
ApplicationWindow{
    id: kPlayer
    width: 1024
    height: 650
    visible: true
    title: getVedioName(fd.fileUrl.toString())
    //获取影音名称
    function getVedioName(str)
    {
        var url = fd.fileUrl.toString();
        var strList = new Array(1);
        strList = url.split("/");
        var name = strList[strList.length-1];
        return name;
    }
    property bool bgimg_visible: false
    property bool bggif_visible: false

    Column{
        Rectangle{
            id: screen
            color: "black"
            width: kPlayer.width
            height: kPlayer.height-50
            Image{
                id: img
                visible: ("" === getVedioName(fd.fileUrl.toString())?true:false )
                source: "./Images/KPlayer_new.png"
                anchors.fill: parent
            }
            Image {
                id: img_static
                visible: bgimg_visible
                source: "./Images/Music.gif"
                anchors.fill: parent
            }

            Rectangle {
                id: img_gif
                color: "transparent"
                visible: bggif_visible
                anchors.fill: parent
                AnimatedImage {
                    source: "./Images/Music.gif"
                    anchors.fill: parent
                }
            }

            MediaPlayer{
                id: player
                source: fd.fileUrl
                autoPlay: true
                volume: voice.value
                onStopped: {
                    playtimers.text = "00:00:00/00:00:00"
                    img_gif.visible = false;
                    img.visible = true;
                }
                onPlaying: {
                    console.log("is play status ")
                    if (showgif(getVedioName(fd.fileUrl.toString())))
                    {
                        img.visible = false;
                        bgimg_visible = false;
                        bggif_visible = true;
//                        img_gif.playing = true;
                        console.log("is mp3 in play", bggif_visible);
                    }
                }
                onPaused: {
                    console.log("is pause status ")
                    if (showgif(getVedioName(fd.fileUrl.toString())))
                    {
                        img.visible = false;
                        bggif_visible = false;
                        bgimg_visible = true;
                        console.log("is mp3 in pause")
                    }
                }
            }
            VideoOutput {
                anchors.fill: parent
                source: player
                fillMode: VideoOutput.Stretch
            }
        }
        Rectangle{
            id: control
            color: "#80202020"
            border.color: "gray"
            border.width: 1
            width: kPlayer.width
            height: 20
            Row {
                spacing: 10
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: 10
                anchors.left: parent.left
                //调节播放速度
                Slider{
                    id: playPos
                    property bool sync: false
                    width: kPlayer.width*0.75
                    height: 10
                    maximumValue: player.duration
                    minimumValue: 0
                    value: player.position
                    anchors.verticalCenter: parent.verticalCenter
                    stepSize: 1000
                    style: SliderStyle {
                        groove: Rectangle {
                            width: kPlayer.width*0.8
                            height: 8
                            color: "gray"
                            radius: 2
                            Rectangle {
                                anchors.left: parent.left
                                anchors.top: parent.top
                                anchors.bottom: parent.bottom
                                width: player.duration>0?parent.width*player.position/player.duration:0
                                color: "blue"
                            }
                        }
                        handle: Rectangle {
                            anchors.centerIn: parent
                            color: control.pressed ? "white" : "darkgray"
                            border.color: "gray"
                            border.width: 2
                            implicitWidth: 15
                            implicitHeight: 15
                            radius: 7.5
                            Rectangle {
                                width: parent.width-8
                                height: width
                                radius: width/2
                                color: "blue"
                                anchors.centerIn: parent
                            }
                        }

                    }
                    onValueChanged: {
                        if(!sync)
                        {
                            player.seek(value);
                            console.log("ValueChanged!")
                        }
                    }

                    Connections {
                        target: player
                        onPositionChanged: {
                            playPos.sync = true;
                            playPos.value = player.position;
                            playPos.sync = false;
                            if (1 === btn_play_pause.status)
                                playtimers.text = currentTime(player.position)+"/"+currentTime(player.duration)
//                            console.log("onPositionChanged will update sth!")
                        }
                    }
                    Connections {
                        target: player
                        onStopped: {
                            playPos.value = 0;
                            playtimers.text = "00:00:00/00:00:00"
                            btn_play_pause.status = 0;
                            btn_play_pause.iconImage = "./Images/play.png"
                            console.log("recevice stop signals")
                        }
                    }

                    //点击鼠标设置播放位置
                    MouseArea {
                        property int pos
                        anchors.fill: parent
                        onClicked: {
                            if (player.seekable)
                            {
                                pos = player.duration * mouse.x/parent.width
                                btn_play_pause.status = 1;
                                btn_play_pause.iconImage = "Images/pause.png";
                                player.play();
                            }
                            player.seek(pos)
                            playPos.value = pos;
                        }
                    }
                }
                Image{
                    width: 15
                    height: 15
                    source: "./Images/voice.png"
                    anchors.verticalCenter: parent.verticalCenter
                }
                //调节音量
                Slider{
                    id:voice
                    width: kPlayer.width*0.2
                    height: 10
                    value: player.volume
                    stepSize: 0.1
                    maximumValue: 1
                    minimumValue: 0
                    anchors.verticalCenter: parent.verticalCenter
                    style: SliderStyle {
                        groove: Rectangle {
                            implicitWidth: kPlayer.width*0.2
                            implicitHeight: 8
                            color: "gray"
                            radius: 2
                            Rectangle {
                                anchors.left: parent.left
                                anchors.top: parent.top
                                anchors.bottom: parent.bottom
                                width: player.volume>0?parent.width*player.volume:0
                                color: "blue"
                            }
                        }
                        handle: Rectangle {
                            anchors.centerIn: parent
                            color: control.pressed ? "white" : "darkgray"
                            border.color: "gray"
                            border.width: 2
                            implicitWidth: 15
                            implicitHeight: 15
                            radius: 7.5
                            Rectangle{
                                width: parent.width-8
                                height: width
                                radius: width/2
                                color: "blue"
                                anchors.centerIn: parent
                            }
                        }
                    }
                }
            }
        }
        //控制区域
        Rectangle{
            id:bottom
            color:"#80202020"
            border.color: "gray"
            border.width: 1
            width: kPlayer.width
            height: 30
            Row{
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 10
                spacing: 10
                ButtonImage{
                    enabled: ("" === getVedioName(fd.fileUrl.toString())?false:true )
                    id: btn_play_pause
                    width: 30
                    height: 30
                    property int status: 1  //默认播放
                    iconImage: "./Images/pause.png"
                    onClicked: {
                        if(status === 1)
                        {
                            player.pause();
                            tooltip = "开始";
                            console.log("start")
                            status = 0;
                            iconImage = "./Images/play.png"
                            playtimers.text = currentTime(player.position)+"/"+currentTime(player.duration)
                        }
                        else{
                            player.play() ;
                            tooltip = "暂停";
                            console.log("pause")
                            status = 1;
                            iconImage ="./Images/pause.png"
                            playtimers.text = currentTime(player.position)+"/"+currentTime(player.duration)
                        }
                    }
                }
                ButtonImage {
                    enabled: ("" === getVedioName(fd.fileUrl.toString())?false:true )
                    width: 30
                    height: 30
                    onClicked: {
                        player.stop()
                        playtimers.text = "00:00:00/00:00:00"
                        console.log("stop btn is clicked!")
                    }
                    tooltip: "停止"
                    iconImage: "./Images/stop.png"
                }
                //快进快退10s
                ButtonImage {
                    enabled: ("" === getVedioName(fd.fileUrl.toString())?false:true )
                    width: 30
                    height: 30
                    onClicked: player.seek(player.position+10000)
                    tooltip: "快退"
                    iconImage: "./Images/back.png"
                }
                ButtonImage{
                    enabled: ("" === getVedioName(fd.fileUrl.toString())?false:true )
                    width: 30
                    height: 30
                    onClicked: player.seek(player.position-10000)
                    tooltip: "快进"
                    iconImage: "./Images/pass.png"
                }
                ButtonImage{
                    width: 30
                    height: 30
                    tooltip: "打开文件"
                    onClicked: fd.open()
                    iconImage: "./Images/add.png"
                    FileDialog{
                        id:fd
                        nameFilters: ["Vedio Files(*.avi *.mp4 *rmvb *.rm *.mp3 *.wmv *.mkv)"]  //格式过滤
                        selectMultiple: false
                    }
                }

                Text{
                    id: playtimers
                    anchors.verticalCenter: parent.verticalCenter
                    text: currentTime(player.position)+"/"+currentTime(player.duration)
                    color: "white"
                }                
            }
        }
    }
    //时间格式化
    function currentTime(time)
    {
        var sec = Math.floor(time/1000);  // floor 向下取整，sec将time从毫秒换成秒
        var hours = Math.floor(sec/3600);
        var minutes = Math.floor((sec - hours*3600)/60);
        var seconds = sec-hours*3600 - minutes*60;
        var hh, mm, ss;
        if(hours.toString().length < 2)
            hh = "0"+hours.toString();
        else
            hh = hours.toString();
        if(minutes.toString().length < 2)
            mm = "0"+minutes.toString();
        else
            mm = minutes.toString();
        if(seconds.toString().length < 2)
            ss = "0"+seconds.toString();
        else
            ss = seconds.toString();
        return hh+":"+mm+":"+ss
    }

    function showgif(filename)
    {
        var res = ShowGif.isShowGif(filename);
        if (res === true)
            return true;
        else
            return false;
    }
}

