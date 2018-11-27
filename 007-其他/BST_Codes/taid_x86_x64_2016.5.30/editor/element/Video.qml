import QtQuick 2.3
import "../editor.js" as Editor
import QtMultimedia 5.5

Element{

    Image{
        visible: !previewModel
        width: parent.width/2
        height: parent.height/2
        anchors.centerIn: parent
        source:"qrc:/images/video.png"
        z:-1
    }

    VideoOutput {
        anchors.fill: parent
        source: id_mediaplayer
        fillMode:VideoOutput.Stretch
    }

    MediaPlayer {
        id: id_mediaplayer
        autoLoad: false
        autoPlay: false
        source: panelAttr.m_background
        loops: MediaPlayer.Infinite
        volume: panelAttr.m_volume/100
    }

    onPreviewModelChanged: {
        if(previewModel && panelAttr){
            playVideo();
        }else{
            id_mediaplayer.stop();
        }
    }

    function playVideo(){
        if(id_mediaplayer.PlayingState == MediaPlayer.PlayingState){
            id_mediaplayer.stop();
        }else{
            if(panelAttr.m_background != ""){
                id_mediaplayer.play();
            }
        }
    }
}
