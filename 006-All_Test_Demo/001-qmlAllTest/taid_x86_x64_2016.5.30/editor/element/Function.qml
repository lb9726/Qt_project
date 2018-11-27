import QtQuick 2.5
import "../editor.js" as Editor

Element{
    property string tipInfo: panelAttr.m_tipText
    Image {
        width: parent.width
        height: parent.height
        source: coverImag == "" ? "qrc:/images/icon/3.png" : coverImag
    }
    //    Image {
    //        width: parent.width
    //        height: parent.height*0.8
    //        source: coverImag
    //    }
    //    Text{
    //        anchors.bottom: parent.bottom
    //        height: parent.height/5
    //        text:tipInfo
    //        color: "white"
    //        font.pointSize: 20
    //        verticalAlignment: Text.AlignVCenter
    //        horizontalAlignment: Text.AlignHCenter
    //        anchors.horizontalCenter: parent.horizontalCenter
    //    }

    onPreviewModelChanged: {
        if(previewModel && panelAttr){
            panelAttr.initPreview()
        }else{
            panelAttr.endPreview()
        }
    }
}
