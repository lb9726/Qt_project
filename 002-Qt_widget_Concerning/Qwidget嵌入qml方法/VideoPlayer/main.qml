import QtQuick 2.5
import QtQuick.Window 2.2

Window {
    visible: true
    width: 1064
    height: 768
    color: "black"
    ShowTitle
    {
        x: 351;
        y: 15
    }

    ArrowAnimation
    {
        x: 98;
        y: 131
        width: 156;
        height: 191 //width: 184; height: 162
    }

    FloorNumberShow
    {
        x: 0;
        y: 363
        width: 351;
        height: 173
    }

    TitleAndTime
    {
        x: 884;
        y: 15
    }

    MultiMedia
    {
        x: 350;
        y: 128
        width:675 + 10
        height: 511
    }

    Rollingtext
    {
        x: 348;
        y: 644 + 20
    }

    ShowWifi
    {
        x: 80;
        y: 582
    }
}

