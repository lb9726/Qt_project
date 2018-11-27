import QtQuick 2.5
import "../editor.js" as Editor

Element{
    id:root
    property string folderPath:panelAttr.resRootPath;
    Image{
        source: coverImag
        anchors.fill: parent
        z:1
        opacity: coverImag =="" ? 0 : 1
    }

    Grid {
        enabled: previewModel
        opacity: 0.8
        columns: 3
        anchors.fill: parent
        Repeater{
            model: keyBtnReource
            Item{
                width: root.width/3
                height: root.height/5
                clip:true
                Rectangle{
                    anchors.fill: parent
                    //anchors.margins: l_btnSpacing
                    clip:true
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
        z:2
    }

    Row{
        enabled: previewModel
        opacity: 0.8
        width: root.width
        height: root.height/5
        anchors.bottom: parent.bottom
        z:2
        Repeater{
            model: keyBtnReource2
            Item{
                width: root.width/2
                height: root.height/5
                clip:true
                Rectangle{
                    anchors.fill: parent
                    //anchors.margins: l_btnSpacing
                    clip:true
                    Text{
                        anchors.centerIn: parent
                        text:kIndex
                    }
                    z:-10
                    visible: id_img2.source=="" ? true:false
                }

                Image {
                    id:id_img2
                    z:-1
                    anchors.fill:parent
                    source: kFront == "" ? "":folderPath + kFront
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
    }

    ListModel{
        id:keyBtnReource
        ListElement{kIndex:"55";kFront:"";lBack:""}
        ListElement{kIndex:"56";kFront:"";lBack:""}
        ListElement{kIndex:"57";kFront:"";lBack:""}
        ListElement{kIndex:"52";kFront:"";lBack:""}
        ListElement{kIndex:"53";kFront:"";lBack:""}
        ListElement{kIndex:"54";kFront:"";lBack:""}
        ListElement{kIndex:"49";kFront:"";lBack:""}
        ListElement{kIndex:"50";kFront:"";lBack:""}
        ListElement{kIndex:"51";kFront:"";lBack:""}
        ListElement{kIndex:"65";kFront:"";lBack:""}
        ListElement{kIndex:"48";kFront:"";lBack:""}
        ListElement{kIndex:"66";kFront:"";lBack:""}
    }

    ListModel{
        id:keyBtnReource2
        ListElement{kIndex:"8";kFront:"";lBack:""}
        ListElement{kIndex:"13";kFront:"";lBack:""}
    }

    function slt_getRes(){
        var list = new Array
        if(keyBtnReource.get(0).kFront == ""){
            return list;
        }

        list.push(folderPath);
        var tempString,curItem,i;
        for(i =0;i<keyBtnReource2.count;i++){
            curItem = keyBtnReource2.get(i);
            tempString = curItem.kIndex +":"+ curItem.kFront +":"+ curItem.lBack;
            list.push(tempString);
        }

        for(i =0;i<keyBtnReource.count;i++){
            curItem = keyBtnReource.get(i);
            tempString = curItem.kIndex +":"+ curItem.kFront +":"+ curItem.lBack;
            list.push(tempString);
        }

        return list;
    }

    function slt_Resource(fileList){
        var frontImg;
        var backImg;
        if(fileList.length == 0)
            return;

        if(fileList.length > 1){
            for(var i = 0; i < fileList.length;i++){
                var tmp = fileList[i].split(":");
                if(tmp.length != 3)
                    continue;
                frontImg = tmp[1];// == "" ? "":tmp[1];
                backImg = tmp[2];// == "" ? "":tmp[2];

                if(tmp[0]=="8"){
                    keyBtnReource2.set(0,{kFront:frontImg,lBack:backImg});
                }else if(tmp[0]=="13"){
                    keyBtnReource2.set(1,{kFront:frontImg,lBack:backImg});
                }else{
                    for(var j=0;j<keyBtnReource.count;j++){
                        if(tmp[0] == keyBtnReource.get(j).kIndex){
                            keyBtnReource.set(j,{kFront:frontImg,lBack:backImg});
                            break;
                        }
                    }
                }
            }
        }
    }
}
