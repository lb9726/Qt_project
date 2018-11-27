import QtQuick 2.7
import QtQuick.Window 2.2
//import QtQuick.Dialogs 1.2
import QtQuick.Dialogs 1.1
Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    ButtonSelfDefineLong {
        btext: "hello"
        bwidth: 50
        bheight: 20
        onButtonClick: {
            console.log("btn is clicked");
            addVideo();
        }
    }
    FileDialog {
        id: id_pickVideoFileDialog
        title: qsTr("请选择一个视频文件")
        folder: shortcuts.home
        selectMultiple: false
        nameFilters: [ "video files (*.mp4 *.avi *.mkv)" ]
        onAccepted: {/*console.log(id_videoPathLabel.width,id_videoPathLabel.height)*/
//            videoPath = shiftAbsolutePath(id_pickVideoFileDialog.fileUrl.toString());
        }
    }
    function addVideo() {
        id_pickVideoFileDialog.open();
    }
}
