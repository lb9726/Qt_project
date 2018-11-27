import QtQuick 2.4
import QtMultimedia 5.5
// 主画面坐标为： x: 350; y: 128

/*Item*/
Rectangle {
    id:screen
    color: "transparent"
    MediaPlayer{
        id:player
        source: "file:///home/libo/Desktop/KONE_Demo_CanUse/Test1.mp4"
        autoLoad: false
        autoPlay: true    // 当AutoPlay 设置为true的时候，如果视频文件存在，就会直接播放视频
        volume: 0.6
        loops: MediaPlayer.Infinite
        onStopped:
        {
            console.log("the media is end!")
        }
    }
    VideoOutput {
        anchors.fill: parent
        source: player
        fillMode: VideoOutput.Stretch
    }
}




