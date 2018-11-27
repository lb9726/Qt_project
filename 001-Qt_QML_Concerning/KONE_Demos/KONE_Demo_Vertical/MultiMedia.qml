import QtQuick 2.4
import QtMultimedia 5.5
// 主画面坐标为： x: 350; y: 128

/*Item*/
Rectangle {
    id:screen
    color: "transparent"
    MediaPlayer{
        id:player
        //source: "file:///home/devin/Desktop/Repository/Qt_project/Qt_project/AutoPlayer-new/videos/Test2.mp4"
//        source: "file:///home/root/ThreeKingdoms.mp4"
        source: "file:///home/root/hengxian.mp4"
//        source: 'file:///home/devin/Desktop/song_video/爱奇艺早班机：众歌手唱砸歌曲成音乐车祸现场.mp4'
        autoLoad: false
        autoPlay: true    // 当AutoPlay 设置为true的时候，如果视频文件存在，就会直接播放视频
        volume: 0.6
        loops: MediaPlayer.Infinite
        onStopped: {
            console.log("the media is end!")
        }
    }
    VideoOutput {
        anchors.fill: parent
        source: player
        fillMode:VideoOutput.Stretch
    }
}




