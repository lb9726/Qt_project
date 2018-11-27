import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.2

Item {
    id:arwidth
    property string videoPath: "" //本地视频文件路径
    anchors.fill: parent
    Rectangle {
        id: labelItem
        width: parent.width
        color: "transparent"
        y: 15
        height: updateMedia_id.height
        Label {
            id: updateMedia_id
            text: qsTr("多媒体更新")
            anchors.centerIn: parent
        }
    }

    Rectangle {
        id: radioButtonItem
        width: parent.width
        height: 66
        color: "transparent"
        y: 55
        ColumnLayout {
            anchors.centerIn: parent
            id: radioButtonColumnLayout1
            RadioButton {
                id: normal_show
                checked: true
                text: qsTr("正常显示")
            }
            RadioButton {
                id: fullscreen_show
                text: qsTr("全屏显示")
            }
        }
    }

    Rectangle {
        id: radioButton_videoOrpicture
        width: parent.width
        y: 140
        RowLayout {
            spacing: 70
            RadioButton {
                id: video_text_btn
                text: qsTr("视频")
                checked: true
            }
            RadioButton {
                id: picture_text_btn
                text: qsTr("图片")
            }
        }
    }

    Item {
        id: select_pictureOrvideo
        width: parent.width
        height: 100
        visible: video_text_btn.checked
        y: 200
        ColumnLayout {
            id: column_select
            anchors.fill: parent
            Label {
                id: id_videoPathLabel
                text: videoPath
                visible: text.length > 0
                // anchors.verticalCenter: parent.verticalCenter
                // verticalAlignment: Text.AlignVCenter
                // anchors.horizontalCenter: parent.horizontalCenter
                // horizontalAlignment: Text.AlignHCenter
                // verticalAlignment: Text.AlignVCenter
                wrapMode: Text.WordWrap
                elide: Text.ElideMiddle
            }
//            Rectangle {
//                color: "transparent"
//                height: 80
//            }
            Button {
                id: select_btn_video
                anchors.centerIn: parent
                text: qsTr("选择视频")
                onClicked: {
                    addVideo();
                }
            }
        }
    }
    Item {
        id: id_pictureArea
        visible: picture_text_btn.checked
        width: parent.width
        height: 100
        y: 200
        Button {
            text: qsTr("选择图片")
            anchors.centerIn: parent
            visible: id_pickedPictureModel.count === 0
            onClicked: {
                addPictures();
            }
        }
    }
    Item {
        id: picture_rectangle
        width: 200
        height: 270
        x: 30
        y: 185
        ColumnLayout {
//            width: 180
//            height: 230
            anchors.centerIn: parent
//            spacing: 10
            visible: (id_pickedPictureModel.count != 0 && picture_text_btn.checked)
            Item {
                width: 200
                height: 200
                Rectangle {
                    color: "transparent"
                    border.width: 1
                    border.color: "gray"
                    opacity: 0.6
                    anchors.fill: parent
                }
                ListView {
                    id:id_pictureListView
                    width: 200
                    height: 200
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
                        MouseArea {
                            anchors.fill:parent
                            onClicked: {
                                id_pictureListView.currentIndex = index;
                            }
                        }
                    }
                }
            }

            RowLayout {
                height: 40
                spacing: 6
                anchors.horizontalCenter: parent.horizontalCenter
                Button {
                    width: 30
                    text:qsTr("添加")
                    onClicked: addPictures();
                }
                Button {
                    width: 30
                    text:qsTr("删除")
                    onClicked:deletePicture(id_pictureListView.currentIndex);
                }
                Button {
                    width: 30
                    text:qsTr("清空")
                    onClicked:clearPictures();
                }
            }
        }

    }

    function addVideo(){
        id_pickVideoFileDialog.open();
    }
    function shiftAbsolutePath(path){
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
        title: qsTr("请选择一个视频文件")
        folder: shortcuts.home
        selectMultiple: false
        nameFilters: [ "video files (*.mp4 *.avi)" ]
        onAccepted: {console.log(id_videoPathLabel.width,id_videoPathLabel.height)
            videoPath = shiftAbsolutePath(id_pickVideoFileDialog.fileUrl.toString());
        }
    }
    ListModel {
        id:id_pickedPictureModel
        //ListElement {path: "C:/"}
    }
    function addPictures() {
        id_pickImageFileDialog.open();
    }
    function deletePicture(index)  {
        id_pickedPictureModel.remove(index);
    }

    function clearPictures() {
        id_pickedPictureModel.clear();
    }
    FileDialog {
        id: id_pickImageFileDialog
        title: qsTr("请选取图片")
        folder: shortcuts.home
        selectMultiple:true
        nameFilters: [ "Image files (*.png *.jpg)" ]
        onAccepted: {
            for( var i=0;i<id_pickImageFileDialog.fileUrls.length;i++)
            {
                id_pickedPictureModel.append({"path": shiftAbsolutePath(id_pickImageFileDialog.fileUrls[i])});
            }
        }
    }

    Component.onCompleted: {
        console.log("labelItem = ", labelItem.width, labelItem.height)
        console.log("radioButtonItem = ", radioButtonItem.width, radioButtonItem.height)
        console.log("radioButtonColumnLayout1 = ", radioButtonColumnLayout1.width, radioButtonColumnLayout1.height)
        console.log("normal_show = ", normal_show.width, normal_show.height)
        console.log("radioButton_videoOrpicture = ", radioButton_videoOrpicture.width, radioButton_videoOrpicture.height)
        console.log("video_text_btn = ", video_text_btn.width, video_text_btn.height)
        console.log("picture_text_btn = ", picture_text_btn.width, picture_text_btn.height)
        console.log("column_select = ", column_select.width, column_select.height)
        console.log("select_pictureOrvideo = ", select_pictureOrvideo.width, select_pictureOrvideo.height)
        console.log("select_btn_video = ", select_btn_video.width, select_btn_video.height)
        console.log("id_videoPathLabel = ", id_videoPathLabel.width, id_videoPathLabel.height)
    }
}
