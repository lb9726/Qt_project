import QtQuick 1.1

Image {
    property int actionFlag: -1
    fillMode: Image.PreserveAspectFit
    property int tinkInterval:1000

    //visible: false ///Icon默认都是不可见的
    Timer{
        id:id_tinkTimer
        interval: tinkInterval
        repeat: true
        onTriggered: {
            if(opacity == 1){
                opacity = 0;
            }else{
                opacity = 1
            }
        }
    }

    //-1——不显示，0——静态显示，1-1秒闪烁2次，2-1秒闪烁1次
    function action(flag){
        if(actionFlag === flag)
            return;
        id_tinkTimer.stop();
        switch(flag){
        case -1:
            visible = false;
            break;
        case 0:
            visible = true;
            opacity = 1;
            break;
        case 1:
            tinkInterval = 500;
            id_tinkTimer.start();
            break;
        case 2:
            tinkInterval = 100;
            id_tinkTimer.start();
            break;
        default:
            break;
        }
        actionFlag = flag;
    }

    function setIcon(icon){
        source = icon;
    }
}
