import QtQuick 2.5
import "../editor.js" as Editor

Element{
    property string l_bottonOnImg :panelAttr.m_bottonOnImg
    property string l_bottonOffImg:panelAttr.m_bottonOffImg
    property real l_btnSpacing:(1-panelAttr.m_btnSpacing)/2
    property bool l_withDown:panelAttr.m_withDown
    property bool l_withUp:panelAttr.m_withUp

    Image{
        enabled: previewModel
        anchors.fill: parent
        source: coverImag
        z:-10
        Image{
            visible: l_withUp
            width: parent.width
            height: {
                if(l_withUp&&l_withDown){
                    return parent.height*l_btnSpacing
                }else if (l_withUp){
                    return parent.height
                }else{
                   return 0
                }
            }
            anchors.top:parent.top
            source: l_bottonOffImg
            MouseArea{
                anchors.fill: parent
                onPressed: {
                    parent.source = l_bottonOnImg
                }
                onReleased: {
                    parent.source = l_bottonOffImg
                }
            }
        }
        Image{
            rotation: 180
            visible: l_withDown
            width: parent.width
            height: {
                if(l_withUp&&l_withDown){
                    return parent.height*l_btnSpacing
                }else if (l_withDown){
                    return parent.height
                }else{
                   return 0
                }
            }
            anchors.bottom: parent.bottom
            source:l_bottonOffImg
            MouseArea{
                anchors.fill: parent
                onPressed: {
                    parent.source = l_bottonOnImg
                }
                onReleased: {
                    parent.source = l_bottonOffImg
                }
            }
        }
    }
}
