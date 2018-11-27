import QtQuick 1.1

UiElement{
    id:id_arrow

    property string upArrow: ""
    property string downArrow: ""
    property string doubleArrow: ""
    property string arrowUrl: ""  //rootpath+"arrow/1.png"

    MouseArea{
        anchors.fill: parent
        onClicked: {
            arrowCtrl(1,true)
        }
    }

    clip:true
    Column{
        id:id_scrollArrow
        width: id_arrow.width
        height: id_arrow.height
        Image {
            id:arrorimage
            width: id_arrow.width
            height: id_arrow.height
            source: arrowUrl
        }
        Image {
            width: id_arrow.width
            height: id_arrow.height
            source: arrowUrl
        }
    }

    NumberAnimation {
        id:animatUp
        target:id_scrollArrow
        property: "y"
        from:0; to:-id_arrow.height ;
        alwaysRunToEnd: true
        duration: 2000 //l_speedLeve;
        loops: Animation.Infinite
    }

    NumberAnimation {
        id:animatDown
        target:id_scrollArrow
        property: "y"
        alwaysRunToEnd: true
        from: -id_arrow.height; to: 0;
        duration: 2000 //l_speedLeve;
        loops: Animation.Infinite
    }

    function arrowCtrl(orienteRun,isScroll){
        animatUp.stop();
        animatDown.stop();
        if(orienteRun === 0){
            arrowUrl = ""
        }else if(orienteRun === 1){
            arrowUrl = upArrow;
            if(isScroll)
                animatUp.start();
        }else if(orienteRun === 2){
            arrowUrl = downArrow;
            if(isScroll)
                animatDown.start();
        }else if(orienteRun === 3){
            arrowUrl = doubleArrow;
        }
        else{
            animatUp.stop();
            animatDown.stop();
            arrowUrl = "";
        }
    }
}
