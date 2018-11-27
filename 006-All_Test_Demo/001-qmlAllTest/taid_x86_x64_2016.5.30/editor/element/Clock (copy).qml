import QtQuick 2.5
import "../editor.js" as Editor

Element{
    property string dateString: "2015-10-25"
    property string timeString: "19:20:20"
    property bool l_ShowSecond: panelAttr.m_ShowSecond  //bool
    property string l_DateStyle: panelAttr.m_DateStyle //string

    property real l_StyleScale: panelAttr.m_StyleScale
    property bool l_ColokStyle: panelAttr.m_ColokStyle
    property bool l_Direction:panelAttr.m_Direction
    property bool l_btnSpacing:panelAttr.m_btnSpacing


    onL_DateStyleChanged: {
        upDateClock();
    }
    onL_ShowSecondChanged: {
        upDateClock();
    }

    Timer {
        interval: 1000; running: true; repeat: true
        onTriggered: {
            upDateClock();
        }
    }

    function upDateClock(){
        var date = new Date()
        var mt = ((date.getMonth()+1) >9 ? (date.getMonth()+1):("0"+(date.getMonth()+1)))
        var d = (date.getDate() >9 ? date.getDate():("0"+(date.getDate())));

        if(l_DateStyle=="yyyy-MM-dd"){
            dateString = date.getFullYear()+"-"+mt+"-"+d;
        }else if(l_DateStyle == "yyyy-dd-MM"){
            dateString = date.getFullYear()+"-"+d+"-"+mt;
        }else{

        }

        var h = date.getHours() > 9 ? date.getHours(): ("0"+date.getHours())
        var m = date.getMinutes() > 9 ? date.getMinutes(): ("0"+date.getMinutes())
        var s = date.getSeconds() > 9 ? date.getSeconds(): ("0"+date.getSeconds())

        if(l_ShowSecond){
            timeString = h+":"+m+":"+s;
        }else{
            timeString = h+":"+m
        }
    }

    Item{
        visible: l_Direction
        anchors.fill: parent
        Rectangle{
            width: parent.width
            height: parent.height*l_StyleScale
            y: l_ColokStyle ? parent.height*(1-l_StyleScale) :0
            color: "green"
            Text{
                id:id_Date
                text:dateString
                font.pixelSize: parent.width/dateString.length
                anchors.centerIn: parent
            }
        }
        Rectangle{
            color: "red"
            width: parent.width
            height: parent.height*(1-l_StyleScale)
            y: l_ColokStyle ? 0 : parent.height*l_StyleScale
            Text{
                id:id_Time
                text:timeString
                font.pixelSize: parent.width/dateString.length
                anchors.centerIn: parent
            }
        }
    }

    Item{
        visible: !l_Direction
        anchors.fill: parent
        Rectangle{
            height:parent.height
            width: parent.width*l_StyleScale
            x: l_ColokStyle ? parent.width*(1-l_StyleScale) :0
            color: "green"
            Text{
                text:dateString
                anchors.centerIn: parent
            }
        }
        Rectangle{
            color: "red"
            height:parent.height
            width: parent.width*(1-l_StyleScale)
            x: l_ColokStyle ? 0 : parent.width*l_StyleScale
            Text{
                text:timeString
                anchors.centerIn: parent
            }
        }
    }

    Image{
        anchors.fill: parent
        source: coverImag
        z:-10
    }
}
