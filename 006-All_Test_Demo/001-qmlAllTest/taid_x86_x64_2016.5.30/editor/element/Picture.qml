import QtQuick 2.3
import "../editor.js" as Editor

Element{    

    Image {
        anchors.fill: parent
        source:panelAttr.m_background
        z:1
    }

    Rectangle{
        anchors.fill: parent
        visible: panelAttr.m_background == "" ? true:false
    }

    onPreviewModelChanged: {
        if(previewModel && panelAttr){
            panelAttr.initPreview()
        }else{
            panelAttr.endPreview()
        }
    }
}
