import QtQuick 2.5
import QtQuick.Controls 1.4
import "../editor.js" as Editor

Element{
    property string folderPath:panelAttr.resRootPath;

    property int l_srtarFl:panelAttr.m_startFlr
    property int l_endFl:panelAttr.m_endFlr
    property int l_rows: panelAttr.m_rows
    property int l_columes:panelAttr.m_columes

    property bool l_layoutUD: panelAttr.layoutUD
    property bool l_layoutHV: panelAttr.layoutHV

    property string l_bottonOnImg :panelAttr.m_bottonOnImg
    property string l_bottonOffImg:panelAttr.m_bottonOffImg


    onL_rowsChanged: buttonLayout();
    onL_columesChanged: buttonLayout();
    onL_srtarFlChanged: buttonLayout();
    //onL_endFlChanged: buttonLayout();
    onL_layoutUDChanged: {
        if(l_layoutUD){
            id_flbtn.verticalLayoutDirection =  GridView.BottomToTop
        }else{
            id_flbtn.verticalLayoutDirection =  GridView.TopToBottom
        }
    }
    onL_layoutHVChanged: buttonLayout();

    Image{
        source: coverImag
        anchors.fill: parent
        z:-10
    }

    GridView{
        id:id_flbtn
        enabled: previewModel
        anchors.fill: parent
        model: flrBtn_model
        clip:true
        cellHeight:id_flbtn.height/l_rows
        cellWidth:id_flbtn.width/l_columes
        //verticalLayoutDirection:GridView.BottomToTop
        interactive: false
        delegate:Item{
            width: id_flbtn.cellWidth
            height: id_flbtn.cellHeight
            clip:true

            Rectangle{
                anchors.fill: parent
                clip:true
                anchors.margins: 2
                Text{
                    anchors.centerIn: parent
                    text:kIndex
                }
                z:-10
                visible: id_img.source=="" ? true:false
            }

            Image {
                id:id_img
                z:-1
                anchors.fill:parent
                source: kFront == "" ? "":folderPath + kFront;
                MouseArea{
                    anchors.fill:parent
                    onPressed: {
                        parent.source = (lBack == "" ? "":folderPath + lBack);
                    }
                    onReleased: {
                        parent.source = (kFront == "" ? "":folderPath + kFront);
                    }
                    onClicked: {
                        //console.log(kIndex)
                    }
                }
            }
        }
    }

    ListModel{
        id:flrBtn_model
    }

    Component.onCompleted: {
        buttonLayout();
        if(panelAttr.hasOwnProperty("pullResource")){
            panelAttr.realPull();
        }
    }

    function buttonLayout(){
        var flrNumber = l_rows*l_columes;
        flrBtn_model.clear();

        if(l_layoutHV){
            for(var i = 0;i< flrNumber;i++){
                flrBtn_model.append({"kIndex":l_srtarFl + i,"kFront":"","lBack":""})
            }
        }else{
            for(i = 0;i< flrNumber;i++){
                //flrBtn_model.append({"kIndex":l_srtarFl + Math.floor(i/l_columes)+i%l_columes,"kFront":"","lBack":""})
                flrBtn_model.append({"kIndex":l_srtarFl +i,"kFront":"","lBack":""})
            }

            for(i = 0;i<l_rows;i++){
                for(var j = 0;j< l_columes ;j++){
                    flrBtn_model.set(i*l_columes+j,{"kIndex":j*l_rows+i+l_srtarFl})
                }
            }
        }



    }

    function slt_getRes(){
        var list = new Array
        list.push(folderPath);
        var tempString,curItem,i;
        for(i =0;i<flrBtn_model.count;i++){
            curItem = flrBtn_model.get(i);
            tempString = curItem.kIndex +":"+ curItem.kFront +":"+ curItem.lBack;
            list.push(tempString);
        }
        return list;
    }

    function slt_Resource(fileList){
        var frontImg;
        var backImg;
        for(var i = 0; i < fileList.length;i++){
            var tmp = fileList[i].split(":");
            if(tmp.length != 3)
                continue;
            frontImg = tmp[1] == "" ? "":tmp[1];
            backImg = tmp[2] == "" ? "":tmp[2];
            for(var j=0;j < flrBtn_model.count;j++){
                if(tmp[0] == flrBtn_model.get(j).kIndex){
                    flrBtn_model.set(j,{kFront:frontImg,lBack:backImg});
                    break;
                }
            }
        }
    }
}
