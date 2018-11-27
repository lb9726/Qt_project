import QtQuick 2.4
import QtMultimedia 5.0
// 主画面坐标为： x:291; y:112

Rectangle {
    property alias screen:screen
    id:screen
    color:"transparent"
    width:697
    height: 576

    MediaPlayer{
        id:player
        //source: "file:///home/devin/Desktop/Repository/Qt_project/Qt_project/AutoPlayer-new/videos/Test2.mp4"
//        source: "file:///home/root/ThreeKingdoms.mp4"
        //source: "file:///home/root/Test1.mp4"
        source: "file:///home/devin/Desktop/fushanxing.rmvb"
        autoLoad: false
        autoPlay: true    // 当AutoPlay 设置为true的时候，如果视频文件存在，就会直接播放视频
        volume: 0.4
        loops: MediaPlayer.Infinite
        onStopped: {
            console.log("the media is end!")
        }
    }
    VideoOutput {
        anchors.fill: parent
        source: player
        fillMode: VideoOutput.Stretch
    }
}




