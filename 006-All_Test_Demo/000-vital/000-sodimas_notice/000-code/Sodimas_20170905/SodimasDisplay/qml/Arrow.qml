import QtQuick 1.1

UiElement {
    id: id_arrow

    property string upArrow: ""
    property string downArrow: ""
    property string doubleArrow: ""
    property string arrowUrl: ""  //rootpath+"arrow/t1.png"
    property int inervalPerScroll: 2000 ///Default value is 2 seconds

    property bool isDoubleArrow:false

    clip: true
    Column {
        id: id_scrollArrow
        width: id_arrow.width
        height: id_arrow.height
        Image {
            id: arrorimage
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

    Image {
        width: id_arrow.width
        height: id_arrow.height
        source: doubleArrow
        visible: isDoubleArrow
        //z: visible ? 1: -1
        z: 1
    }

    NumberAnimation {
        id: animatUp
        target: id_scrollArrow
        property: "y"
        from: 0; to: -id_arrow.height ;
        alwaysRunToEnd: true
        duration: inervalPerScroll //l_speedLeve;
        loops: Animation.Infinite
    }

    NumberAnimation {
        id: animatDown
        target: id_scrollArrow
        property: "y"
        alwaysRunToEnd: true
        from: -id_arrow.height; to: 0;
        duration: inervalPerScroll //l_speedLeve;
        loops: Animation.Infinite
    }

    function arrowCtrl(orienteRun, isScroll)
    {
        ///console.log("qml arrowCtrl");
        animatUp.stop();
        animatDown.stop();
        if (orienteRun === 0)
        {
            arrowUrl = "";
            isDoubleArrow = false;
        }
        else if (orienteRun === 1)
        {
            isDoubleArrow = false;
            arrowUrl = upArrow;
            if (isScroll)
            {
                animatUp.start();
            }
        }
        else if (orienteRun === 2)
        {
            isDoubleArrow = false;
            arrowUrl = downArrow;
            if (isScroll)
            {
                animatDown.start();
            }
        }
        else if (orienteRun === 3)
        {
            arrowUrl = "";
            isDoubleArrow = true;
        }
        else
        {
            isDoubleArrow = false;
            animatUp.stop();
            animatDown.stop();
            arrowUrl = "";
        }
    }

    function arrowSpeed(interval) {
        if (interval>1000&&interval<60000) {
            ///inervalPerScroll = interval
            inervalPerScroll = interval/2
        }
    }
}


//function arrowCtrl(orienteRun,isScroll) {
//    ///console.log("qml arrowCtrl");
//    animatUp.stop();
//    animatDown.stop();
//    if (orienteRun === 0) {
//        arrowUrl = ""
//    }else if (orienteRun === 1) {
//        arrowUrl = upArrow;
//        if (isScroll)
//            animatUp.start();
//    }else if (orienteRun === 2) {
//        arrowUrl = downArrow;
//        if (isScroll)
//            animatDown.start();
//    }else if (orienteRun === 3) {
//        arrowUrl = doubleArrow;
//    }
//    else {
//        animatUp.stop();
//        animatDown.stop();
//        arrowUrl = "";
//    }
//}
