import QtQuick 2.5
import "../editor.js" as Editor

Element{
    id:id_arrow
    property bool orienteRun: panelAttr.oriention
    property int l_speedLeve: panelAttr.speedLeve

    Rectangle{
        anchors.fill: parent
        visible: coverImag == "" ? true :false
    }

    Item{
        anchors.fill: parent
        clip:true
        Item{
            id:id_scrollArrow
            width: id_arrow.width
            height: id_arrow.height
            Image {
                id:arrorimage
                width: id_arrow.width
                height: id_arrow.height
                source: coverImag

            }
            Image {
                width: id_arrow.width
                height: id_arrow.height
                source: coverImag
                y:id_scrollArrow.height
            }
        }
    }

    onPreviewModelChanged: {
        if(previewModel){
            animationCtrl();
        }else{
            animatUp.stop();
            animatDown.stop();
        }
    }

    onOrienteRunChanged: {
        if(!previewModel)
            return;
        animationCtrl();
    }

    function animationCtrl(){
        if(orienteRun){
            animatDown.stop();
            animatUp.start();
        }else{
            animatUp.stop();
            animatDown.start();
        }
    }

    NumberAnimation {
        id:animatUp
        target:id_scrollArrow
        property: "y"
        from:0; to:-id_arrow.height ;
        alwaysRunToEnd: true
        duration: 1000 //l_speedLeve;
        loops: Animation.Infinite
    }

    NumberAnimation {
        id:animatDown
        target:id_scrollArrow
        property: "y"
        alwaysRunToEnd: true
        from: -id_arrow.height; to: 0;
        duration: 1000 //l_speedLeve;
        loops: Animation.Infinite
    }
}
