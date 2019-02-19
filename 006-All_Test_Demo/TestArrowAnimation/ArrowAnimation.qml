import QtQuick 2.5

Rectangle {
    color: "transparent"
    border.color: "black"
    border.width: 1
    property string imagesource: 'images/ArrowUp1.png'
    property bool isDown: false
    property int i: 0
    property int mRotate: 0;
    property int mDuration: 2000 /*900 1200 1500 2000*/
    id: idShowarea
    clip: true
    rotation: mRotate
    Item {
        id: idImageItem
        width: idShowarea.width
        height: idShowarea.height
        Image {
            id: imagefirstpos
            width: idShowarea.width
            height: idShowarea.height
            source: imagesource
            y: 0
        }
        Image {
            width: idShowarea.width
            height: idShowarea.height
            source: imagesource
            y: idShowarea.height
        }
    }

    NumberAnimation {
        id: idAnimaUp
        target: idImageItem
        property: "y"
        from: 0
        to: -idShowarea.height
        alwaysRunToEnd: false
        duration: mDuration
        loops: Animation.Infinite
    }

    NumberAnimation {
        id: idAnimaDw
        target: idImageItem
        property: "y"
        from: -idShowarea.height
        to: 0
        alwaysRunToEnd: false
        duration: mDuration
        loops: Animation.Infinite
    }

    Timer {
        interval: 5000
        running: true
        repeat: true
        onTriggered: {
            changepic();
        }
    }

    function changepic()
    {
        setRotate();
        if (0 == i)
        {
            startDown();
            i = 1;
        }
        else if (1 == i)
        {
            startUp();
            i = 0;
        }
    }

    function startUp()
    {
        imagesource = 'images/ArrowUp1.png';
        isDown = false;
        idAnimaDw.stop();
        idAnimaUp.restart();
//        console.log("mRotate = ", mRotate);
    }

    function startDown()
    {
        imagesource = 'images/ArrowDown1.png';
        isDown = true;
        idAnimaUp.stop();
        idAnimaDw.restart();
//        console.log("mRotate = ", mRotate);
    }

    function setRotate()
    {
        if (0 == mRotate)
        {
            mRotate = 90;
            mDuration = 900;
            setXAndY(50, 80);
        }
        else if (90 == mRotate)
        {
            mRotate = 180;
            mDuration = 1200;
            setXAndY(100, 80);
        }
        else if (180 == mRotate)
        {
            mRotate = 270;
            mDuration = 1500;
            setXAndY(150, 80);
        }
        else if (270 == mRotate)
        {
            mRotate = 0;
            mDuration = 2000;
            setXAndY(200, 80);
        }
        console.log("mRotate = ", mRotate);
    }

    function setXAndY(pX, pY)
    {
        idShowarea.x = pX;
        idShowarea.y = pY;
    }

    Component.onCompleted: {
        startUp();
    }
}

