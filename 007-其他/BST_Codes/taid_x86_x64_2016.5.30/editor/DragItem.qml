/**
* It's a vessel for holding draggable item and the initial image
*/

import QtQuick 2.5
import "editor.js" as Code

Item{
    id: paletteItem
    property string dragQmlFile
    property string image
    property string dragImage
    property string panelName
    property string toolName
    property Item parentItem
    property int allowIndex: 0
    property int createIndex: 0
    clip: true

    enabled: allowIndex > 0 ? true :false
    opacity: enabled ? 1 :0.4

    Column{
        anchors.fill: parent
        spacing: width/10
        Image {
            source: image
            width: parent.width/2
            height: parent.height/2
            anchors.horizontalCenter:  parent.horizontalCenter
        }
        Text {
            text: toolName
            font.pixelSize: parent.width/5
            anchors.horizontalCenter: parent.horizontalCenter
            color: "white"
            font.family:"微软雅黑"
        }
    }

    MouseArea {
        anchors.fill: parent
        onPressed: {
            if(allowIndex >0)
                Code.startDrag(mouse);
        }
        onPositionChanged: {
            Code.continueDrag(mouse);
        }
        onReleased: {
            //createIndex ++;
            Code.setPanelName(panelName,createIndex)
            Code.endDrag(mouse);
        }
    }

    Component.onCompleted:{
        Code.init(parentItem, dragQmlFile, dragImage);
    }
}



