import QtQuick 2.0
import QtMultimedia 5.0
Item {
    width:350
    height: 350
    MediaPlayer {
           id: mediaplayer
           source: "./Test1.mp4"
       }

       VideoOutput {
           anchors.fill: parent
           source: mediaplayer
       }

       MouseArea {
           id: playArea
           anchors.fill: parent
           onPressed: mediaplayer.play();
       }
}

