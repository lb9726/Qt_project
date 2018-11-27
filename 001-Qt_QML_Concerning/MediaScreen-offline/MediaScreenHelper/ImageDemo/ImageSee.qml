import QtQuick 2.2
import QtQuick.Window 2.1
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import QtQuick.Dialogs 1.1

Window{
    property int image_index: 0
    visible: true;
    width: 600;
    height: 480;

    minimumWidth: 480;
    minimumHeight: 380;

    BusyIndicator{
        id:busy;
        running: true;
        anchors.centerIn: parent;
        z:2;

    }

    Text {
        id:statusLabel;
        visible: false;
        anchors.centerIn: parent;
        z:3;
    }

    Rectangle {
        anchors.centerIn: parent;
        width: parent.width - 50;
        height: parent.height - 50;
        x: 15
        y: 15
        Image {

        }
        Image {

        }
        Image{
            id: imageViewer;
            asynchronous: true;
            cache: false;
            anchors.centerIn: parent;
            width: parent.width - 50;
            height: parent.height - 50;
            x: 3
            y: 3
            fillMode: Image.PreserveAspectFit;

            onStatusChanged: {
                if(imageViewer.states === Image.Loading){
                    busy.running = true;
                    statusLabel.visible = false;
                }
                else if(imageViewer.status === Image.Ready){
                    busy.running = false;
                }
                else if(imageViewer.status === Image.Error){
                    busy.running = false;
                    statusLabel.visible = true;
                    statusLabel.text = "ERROR";
                }
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    console.log("MouseArea is clicked")
                    setImageShows();
                }
            }
        }

    }


    Button{
        id:openFile;
        text:"Open";
        anchors.left: parent.left;
        anchors.leftMargin: 8;
        anchors.bottom: parent.bottom;
        anchors.bottomMargin: 8;

        style: ButtonStyle{
            background: Rectangle{
                implicitWidth: 70;
                implicitHeight: 25;
                color:control.hovered?"#c0c0c0":"#a0a0a0";
                border.width: control.pressed ? 2 : 1;
                border.color: (control.hovered || control.pressed)? "green":"#888888";
            }
        }

        onClicked: fileDialog.open();
        z:4;
    }


    Text{
        id:imagepath;
        anchors.left: openFile.right;
        anchors.leftMargin: 8;
        anchors.verticalCenter: openFile.verticalCenter;
        font.pixelSize: 18;
    }

    FileDialog{

        id: fileDialog;
        selectMultiple: true
        title: "Please choose a file";
        nameFilters: ["Image Files (*.jpg *.png *.gif)"];
        onAccepted: {
            imageViewer.source = fileDialog.fileUrls[image_index];
            var imageFile = fileDialog.fileUrls[image_index];
            imagepath.text = imageFile.slice(8);
        }
    }
    ListModel {
        id: id_pickedPictureModel
    }
    FileDialog {
        id: id_pickImageFileDialog
        title: qsTr("请选取图片")
        folder: shortcuts.home
        selectMultiple: true
        nameFilters: [ "Image files (*.png *.jpg *.bmp)" ]
        onAccepted: {
            for( var i = 0; i<id_pickImageFileDialog.fileUrls.length; i++)
            {
                id_pickedPictureModel.append({"path": shiftAbsolutePath(id_pickImageFileDialog.fileUrls[i])});
            }
        }
    }

    function addPictures() {
        id_pickImageFileDialog.open();
    }
    function shiftAbsolutePath(path) {
        var pre = path.lastIndexOf("file:///");
        if(pre >= 0) {
            return path.substring(8);
        }else {
            return "";
        }
    }

    function getImageIndex()
    {
        var res = image_index;
        return res;
    }

    function setImageShows()
    {
        var imageFile;
        if (image_index >= fileDialog.fileUrls.length -1)
        {
            console.log("if index = ", image_index)
            image_index = 0;
            imageViewer.source = fileDialog.fileUrls[image_index];
            imageFile = fileDialog.fileUrls[image_index];
            imagepath.text = imageFile.slice(8);
        }
        else
        {
            console.log("else index = ", image_index)
            image_index++;
            imageViewer.source = fileDialog.fileUrls[image_index];
            imageFile = fileDialog.fileUrls[image_index];
            imagepath.text = imageFile.slice(8);
        }
    }
}
