import QtQuick 1.1

UiElement {
    property string baiImage:""
    property string shiImage:""
    property string geImage:""
//    property string baiImage :"file:///opt/theme/sodimas/floor/T51.png"
//    property string shiImage :"file:///opt/theme/sodimas/floor/T51.png"
//    property string geImage:  "file:///opt/theme/sodimas/floor/T51.png"
    clip: true

    property int cellWidth: width/3
    property int offsetintverval: width/6

    Item{
        anchors.fill: parent
        Image{
            id:id_floor_bai
            height: parent.height
            width: cellWidth
            fillMode: Image.PreserveAspectFit
            source: baiImage
            opacity: shiImage == "" ? 0 : 1
            x:0
        }

        Image{
            id:id_floor_shi
            height: parent.height
            width: cellWidth
            fillMode: Image.PreserveAspectFit
            source: shiImage
            opacity: shiImage == "" ? 0 : 1
            x:cellWidth
        }

        Image{
            id:id_floor_ge
            height: parent.height
            width: cellWidth
            fillMode: Image.PreserveAspectFit
            source: geImage
            opacity: geImage == "" ? 0 : 1
            x:2*cellWidth
        }
    }

    function setFloor(ge,shi,bai){
        if(baiImage !== bai)
            baiImage = bai;
        if(shiImage !== shi)
            shiImage = shi;
        if(geImage !== ge)
            geImage = ge;
    }

    function setOffset(geoffset,shioffset,baioffset){
        if(id_floor_bai.x !== baioffset*offsetintverval)
            id_floor_bai.x = baioffset*offsetintverval;
        if(id_floor_shi.x !== shioffset*offsetintverval)
            id_floor_shi.x = shioffset*offsetintverval;
        if(id_floor_ge.x !== geoffset*offsetintverval)
            id_floor_ge.x = geoffset*offsetintverval;    }
}
