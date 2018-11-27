import QtQuick 2.5
import "../editor.js" as Editor

Element{
    id:root
    property string timeString: ""
    property bool l_ShowSecond: panelAttr.m_ShowSecond
    property string l_Split:panelAttr.m_Split

    onPreviewModelChanged: {
        if(previewModel)
            dateTimer.start();
        else
            dateTimer.stop();
    }

    Rectangle{
        anchors.fill: parent
        visible: timeString == "" ? true:false
        z:-10
    }

    Timer{
        id:dateTimer
        interval: 1000
        running: true
        repeat: true
        onTriggered:{
            paraClockTime();
        }
    }

    function paraClockTime(){
        var date = new Date()
        var h = date.getHours() > 9 ? date.getHours(): ("0"+date.getHours())
        var m = date.getMinutes() > 9 ? date.getMinutes(): ("0"+date.getMinutes())
        var s = date.getSeconds() > 9 ? date.getSeconds(): ("0"+date.getSeconds())
        if(l_ShowSecond)
            timeString = h+l_Split+m+l_Split+s;
        else
            timeString = h+l_Split+m;
    }

    Item{
        anchors.fill: parent
        clip: true
        Text{
            id:contentTextId
            anchors.verticalCenter: parent.verticalCenter
            text:timeString
            wrapMode:Text.Wrap
            font:panelAttr.textFont
            color: panelAttr.textColor
        }
    }

    Component.onCompleted: {
        paraClockTime();
        dateTimer.start();
    }
}
