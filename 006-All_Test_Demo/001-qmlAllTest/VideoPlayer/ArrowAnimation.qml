import QtQuick 2.0

Item
{
    property string imagesource: 'images/kone_arrow_up.png'
    property bool direction: true
    property int i: 0
    id: showarea
//    width: 156; height: 191
    clip: true

    Item
    {
        id: id_scrollArrow
        Image
        {
            id: imagefirstpos
            width: showarea.width; height: showarea.height
            source: imagesource
        }
        Image
        {
//            id: imagesecondpos
            width: showarea.width; height: showarea.height
            source: imagesource
            y: showarea.height
        }
    }
    FloorNumberShow
    {
        id: floor
        visible: false
        onFloorChanged:
        {
            showarea.changepic(fnum, upflag);
        }
    }

    function animationCtrl()
    {
        if(direction)
        {
            animaDw.stop();
            animaUp.start();
        }
        else
        {
            animaUp.stop();
            animaDw.start();
        }
//        console.log("go....");
    }
    NumberAnimation
    {
        id: animaUp
        target: id_scrollArrow
        property: "y"
        from:0; to: -showarea.height ;
        alwaysRunToEnd: true
        duration: 1500
        loops: Animation.Infinite
    }

    NumberAnimation
    {
        id: animaDw
        target: id_scrollArrow
        property: "y"
        from: -showarea.height; to: 0;
        duration: 1500
        loops: Animation.Infinite
    }

    function changepic(fnum, upflag)
    {
        if (fnum === 1 && upflag === false)
        {
            // 箭头向下
            direction = false;
            imagesource = 'images/kone_arrow_down.png';
            imagefirstpos.source = imagesource;
            animationCtrl();
//            console.log("fnum = ","upflag = " ,fnum, upflag);
        }
        else if (fnum === 9 && upflag === true)
        {
            //箭头向上
            direction = true;
            imagesource = 'images/kone_arrow_up.png';
            imagefirstpos.source = imagesource
            animationCtrl();
//            console.log("fnum = ","upflag = " ,fnum, upflag);
        }
    }

    Component.onCompleted: animationCtrl()
}

