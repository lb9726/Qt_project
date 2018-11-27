import QtQuick 2.5
import "../editor.js" as Editor

Element{    
    Item {
        anchors.fill: parent
        clip: true
        Text{
            id:floorArea
            anchors.centerIn: parent
            text:panelAttr.m_floorNumber
            wrapMode:Text.Wrap
            font:panelAttr.textFont
            color: panelAttr.textColor
        }
    }
    Rectangle{
        visible: panelAttr.m_floorNumber == "" ? true : false
        anchors.fill: parent
        z:-10
    }
}
