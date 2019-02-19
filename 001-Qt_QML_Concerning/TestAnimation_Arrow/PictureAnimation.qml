import QtQuick 2.5

Item {
    property string imagesource: 'images/kone_arrow_down.png'
    property bool orienteRun: true
    property int i: 0
    id: showarea
//    width: 156; height: 191
    clip: true

    Item {
        id: id_scrollArrow
        width: showarea.width
        height: showarea.height
        Image {
            id: imagefirstpos
            width: showarea.width
            height: showarea.height - 5
            source: imagesource
            y: -5
        }
        Image {
            width: showarea.width
            height: showarea.height - 5
            source: imagesource
            y: showarea.height
        }
    }

    function animationCtrl(){
        if(orienteRun)
        {
            animaUp.stop();
            animaDw.start();
        }
        else
        {
            animaDw.stop();
            animaUp.start();
        }
        console.log("go....");
    }
    NumberAnimation {
        id: animaUp
        target: id_scrollArrow
        property: "y"
        from:0; to:-showarea.height - 2.5;
        alwaysRunToEnd: true
        duration: 1000
        loops: Animation.Infinite
    }

    NumberAnimation {
        id: animaDw
        target: id_scrollArrow
        property: "y"
        from: -showarea.height - 2.5; to: 0;
        duration: 1000
        loops: Animation.Infinite
    }

    Timer {
        interval: 10000; running: true; repeat: true
        onTriggered: {
            changepic();
        }
    }

    function changepic()
    {
        if (0 == i%2 )
        {
            orienteRun = false;
            imagesource = 'images/kone_arrow_up.png';
            imagefirstpos.source = imagesource
            animationCtrl();
        }
        else if (1 == i%2)
        {
            orienteRun = true;
            imagesource = 'images/kone_arrow_down.png';
            imagefirstpos.source = imagesource
            animationCtrl();
        }
        i++;
        if (i >= 2)
            i = 0;
    }

    Component.onCompleted: animationCtrl()

}

