import QtQuick 2.0
import QtMultimedia 5.5

Item
{
    property int playStatus: 1
    MediaPlayer
    {
        id: player
        source: "file:///home/libo/Desktop/KONE_Demo_CanUse/Test1.mp4"
        autoPlay: true
        volume: 0.5
        loops: MediaPlayer.Infinite
        onStopped:
        {
            console.log("is stoped");
        }
    }
    VideoOutput
    {
        width: 600
        height: 400
        source: player
        fillMode: VideoOutput.Stretch
    }
    MouseArea
    {
        anchors.fill: parent
        onClicked:
        {
            playOrPause()
        }
    }
    function playOrPause()
    {
        if (1 == playStatus)
        {
            player.pause()
            playStatus = 0;
            console.log("click will paused");
        }
        else if (0 == playStatus)
        {
            player.play()
            playStatus = 1;
            console.log("click will play");
        }
    }
}
