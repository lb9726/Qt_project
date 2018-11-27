import QtQuick 1.1

UiElement {
    id:id_rootItem

    property string information:"dsdsf\nds\nfds\nfdan\nfdsa\nasdfsadgam\gfgdfg\n\\fasdfs\n"
    clip: true
    Text{
        anchors.fill: parent
        anchors.margins: 6
        color: "white"
        text: information
        verticalAlignment: Text.AlignVCenter
        wrapMode: Text.WordWrap
        maximumLineCount: 6
        smooth: true
    }

    Rectangle{
        anchors.fill: parent
        //opacity: 0.8
        radius: height/6
        border.width:6
        border.color: "blue"
        color: "transparent"
        z:-10
    }


    property int modeFlag: 0

    function updateMsg(msg){
        id_rootItem.visible = true
        information = msg;
    }

    function setMode(flag){
        if(flag <=0 ){
            id_rootItem.visible = true
            id_messageWindowsTimer.stop();
        }else{
            id_messageWindowsTimer.interval = flag;
            id_messageWindowsTimer.restart();
        }
    }

    Timer{
        id:id_messageWindowsTimer
        interval: 0
        repeat: false
        onTriggered: {
            id_rootItem.visible = false
        }
    }
}
