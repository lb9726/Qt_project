import QtQuick 2.5
import "../editor.js" as Editor

Element{
    id:root
    property string l_DateStyle:panelAttr.m_DateStyle
    property string dateString: ""
    property string l_Split:panelAttr.m_Split

    onL_DateStyleChanged: {
        paraClockDate();
    }

    onPreviewModelChanged: {
        if(previewModel)
            dateTimer.start();
        else
            dateTimer.stop();
    }

    Rectangle{
        anchors.fill: parent
        visible: dateString == "" ? true:false
        z:-10
    }

    Timer{
        id:dateTimer
        interval: 1000*60*60
        running: true
        repeat: true
        onTriggered:{
            paraClockDate();
        }
    }

    function paraClockDate(){
        var date = new Date()
        var mt = ((date.getMonth()+1) >9 ? (date.getMonth()+1):("0"+(date.getMonth()+1)))
        var d = (date.getDate() >9 ? date.getDate():("0"+(date.getDate())));
        var year = date.getFullYear();
        if(l_DateStyle=="yyyy-MM-dd"){
            dateString = ""+year+l_Split+mt+l_Split+d;
        }else if(l_DateStyle == "yyyy-dd-MM"){
            dateString = ""+year+l_Split+d+l_Split+mt;
        }else if(l_DateStyle == "dd-MM-yyyy"){
            dateString = d+l_Split+mt+l_Split+year;
        }else if(l_DateStyle == "MM-dd-yyyy"){
            dateString = mt+l_Split+d+l_Split+year;
         }else {
            dateString = ""
        }
    }

    Item{
        anchors.fill: parent
        clip: true
        Text{
            id:contentTextId
            anchors.verticalCenter: parent.verticalCenter
            text:dateString

            wrapMode:Text.Wrap
            font:panelAttr.textFont
            color: panelAttr.textColor
        }
    }

    Component.onCompleted: {
        paraClockDate();
    }
}
