import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.2

Page {
    property alias multimediaSwitcher: id_MultimediaEnabler.checked  //true —— 更新多媒体，false —— 不更新多媒体
    property alias choseVideo: id_videoResource.checked  //true —— 视频，false —— 图片
    property alias isFullScreen: id_fullscreenChecker.checked  //true —— 全屏显示，false —— 非全屏显示

    header: ToolBar {
        height: 50
        RowLayout {
            anchors.fill: parent
            SwitchHelper{
                id:id_MultimediaEnabler
                text: checked ? qsTr("开") :qsTr("关")
            }
            RowLayout{
                visible: id_MultimediaEnabler.checked
                RadioButton {
                    id:id_videoResource
                    checked: true
                    text: qsTr("视频")
                }
                RadioButton {
                    text: qsTr("图片")
                }
            }

            RowLayout{
                visible: id_MultimediaEnabler.checked
                RadioButton {
                    checked: true
                    text: qsTr("正常显示")
                }
                RadioButton {
                    id:id_fullscreenChecker
                    text: qsTr("全屏显示")
                }
            }

        }
    }


    Item {
        enabled: id_MultimediaEnabler.checked
        visible: id_videoResource.checked ? true:false
        opacity: enabled ? 1 :0.5
        anchors.fill: parent

        ColumnLayout{
            anchors.centerIn: parent
            Label{
                id:id_videoPathLabel
                text:videoPath
                visible: text.length>0
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideMiddle
            }

            Button {
                text: qsTr("选择视频")
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    addVideo();
                }
            }
        }
    }

    Item {
        id:id_pictureArea
        enabled: id_MultimediaEnabler.checked
        visible: id_videoResource.checked ? false :true
        opacity: enabled ? 1 :0.5
        anchors.fill: parent

        Button {
            text: qsTr("选择图片")
            anchors.centerIn: parent
            visible: id_pickedPictureModel.count == 0
            onClicked: {
                addPictures();
            }
        }

        ColumnLayout{
            width: 608
            height: 308
            anchors.centerIn: parent
            spacing: 10
            visible: id_pickedPictureModel.count != 0
            Item{
                width: 608
                height: 268
                Rectangle{
                    color: "transparent"
                    border.width: 2
                    border.color: "green"
                    opacity: 0.6
                    anchors.fill: parent
                }
                ListView{
                    id:id_pictureListView
                    width: 600
                    height: 260
                    model: id_pickedPictureModel
                    clip: true
                    anchors.centerIn: parent
                    delegate: Rectangle{
                        height: 30
                        width:id_pictureListView.width
                        clip: true
                        color: index == id_pictureListView.currentIndex ? "grey":"transparent"
                        //opacity: 0.6
                        Text {
                            text: path
                            anchors.centerIn: parent
                        }
                        MouseArea{
                            anchors.fill:parent
                            onClicked: {
                                id_pictureListView.currentIndex = index;
                            }
                        }
                    }
                }
            }

            RowLayout{
                height: 40
                spacing: 10
                anchors.horizontalCenter: parent.horizontalCenter
                Button{
                    width: 150
                    text:"添加"
                    onClicked: addPictures();
                }
                Button{
                    width: 150
                    text:"删除"
                    onClicked:deletePicture(id_pictureListView.currentIndex);
                }
                Button{
                    width: 150
                    text:"清空"
                    onClicked:clearPictures();
                }
            }
        }
    }

    function checkMediaSetting(){
        var res = "";
        if(multimediaSwitcher) {
            if(choseVideo){
                if(videoPath === ""){
                    return "请选择一个视频"
                }
            }else{
                if(id_pickedPictureModel.count == 0){
                    return "请选择图片"
                }
            }
        }
        return "";
    }

    function updateMediaSetting(){
        if(!multimediaSwitcher){
            ConfigureSerialer.updateMultiMedia(false,choseVideo,isFullScreen,"");
        }else{
            var res="";
            if(choseVideo){
                res = videoPath;
            }else{
                for(var i=0;i<id_pickedPictureModel.count;i++){
                    res += id_pickedPictureModel.get(i).path + "*";
                }
            }
            //console.log("media"+res);
            ConfigureSerialer.updateMultiMedia(true,choseVideo,isFullScreen,res);
        }

    }

    function updateMediaSetting2(){
        var res = "";
        //"bool|bool|bool|path1*path2*path3" 第一个bool代表是开启视频更新，第二个bool代表是否全屏，
        //第三个bool代表资源类型，视频或图片，第四个：视频时代表单个视频文件，图片时代表图片路径的连接，以*分割
        if(multimediaSwitcher){
            res = "true|";
            if(isFullScreen){
                res += "true|";
            }else{
                res += "false|";
            }
            if(choseVideo) {
                res += "true|";
                res += videoPath;
            }else{
                res = "false|";
                var tmp;
                for(var i=0;i<id_pickedPictureModel.count;i++){
                    tmp += id_pickedPictureModel.get(i).path + "*";
                }
                res += tmp;
            }
        }else{
            res = "false|false|false|false"
        }

        return res;
    }

    property string videoPath: "" //本地视频文件路径
    ListModel {
        id:id_pickedPictureModel
        //ListElement {path: "C:/"}
    }

    function addVideo(){
        //Component.onCompleted: {
        //console.log(swipeView.width,swipeView.height)
        //console.log(id_pictureArea.width,id_pictureArea.height)
        //}
        id_pickVideoFileDialog.open();
    }

    function addPictures(){
        id_pickImageFileDialog.open();
    }

    function deletePicture(index){
        id_pickedPictureModel.remove(index);
    }

    function clearPictures(){
        id_pickedPictureModel.clear();
    }

    function shiftAbsolutePath(path){
        //console.log("shiftAbsolutePath 1"+path);
        var pre = path.lastIndexOf("file:///");
        if(pre >= 0){
            //console.log("shiftAbsolutePath 2"+path.substring(8));
            return path.substring(8);
        }else{
            return "";
        }
    }

    FileDialog {
        id: id_pickVideoFileDialog
        title: "请选择一个视频文件"
        folder: shortcuts.home
        selectMultiple:false
        nameFilters: [ "Image files (*.mp4 *.avi)", "All files (*)" ]
        onAccepted: {
            videoPath = shiftAbsolutePath(id_pickVideoFileDialog.fileUrl.toString());
        }
    }

    FileDialog {
        id: id_pickImageFileDialog
        title: "请选取图片"
        folder: shortcuts.home
        selectMultiple:true
        nameFilters: [ "Image files (*.png *.jpg)", "All files (*)" ]
        onAccepted: {
            for( var i=0;i<id_pickImageFileDialog.fileUrls.length;i++)
            {
                id_pickedPictureModel.append({"path": shiftAbsolutePath(id_pickImageFileDialog.fileUrls[i])});
            }
        }
    }
}
