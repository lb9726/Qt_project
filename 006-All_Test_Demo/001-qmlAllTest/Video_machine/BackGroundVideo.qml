import QtQuick 2.0
import QtMultimedia 5.5

Item {
    property string mPath: "file:///home/libo/Desktop/000-emulation/Video-And-Song/"
    MediaPlayer {
        id: player
        source: mPath + "BackgroundVideo.mov"
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
