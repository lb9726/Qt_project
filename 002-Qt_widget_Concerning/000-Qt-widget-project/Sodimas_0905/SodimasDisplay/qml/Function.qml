import QtQuick 1.1

UiElement {
    property string funcImage: ""
    Image{
        id:id_floor_bai
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
        source: funcImage
        opacity: funcImage == "" ? 0 : 1
    }

    function setFunction(logoPath){
        //console.log("setFunction ",logoPath);
        if(funcImage !== logoPath)
            funcImage = logoPath;
    }
}
