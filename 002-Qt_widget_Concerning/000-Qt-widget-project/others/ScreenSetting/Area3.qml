import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.2

Item {
    id:arwidth
    property string videoPath: "" //本地视频文件路径
    ColumnLayout {
        id:column1
//        anchors.fill: parent
        ColumnLayout {
            id:column2
            width: parent.width
            RowLayout {
                height: 20
            }
            RowLayout {
                id:row1
                Item {
                    anchors.fill: parent
                    Label {
                        id: updateMedia_id
                        text: qsTr("多媒体更新")
                        anchors.centerIn: parent
                    }
                }
            }

            RowLayout {
                id:row2
                anchors.centerIn: parent
                ColumnLayout {
                    id:row3
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
            RowLayout {
                spacing: 30
                RadioButton {
                    id: select_video
                    text: qsTr("视频")
                    checked: true
                }
                RadioButton {
                    id: select_picture
                    text: qsTr("图片")
                }
            }
        }
        ColumnLayout {
            Item {
                anchors.fill: parent
                ColumnLayout {
//                    anchors.fill: parent
                    width: parent.width
                    visible: select_video.checked
                    Label {
                        id: id_videoPathLabel
                        text: videoPath
                        visible: text.length > 0
//                        anchors.verticalCenter: parent.verticalCenter
//                        verticalAlignment: Text.AlignVCenter
//                        anchors.horizontalCenter: parent.horizontalCenter
//                        horizontalAlignment: Text.AlignHCenter
//                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.WordWrap
                        elide: Text.ElideMiddle
                    }
                    Rectangle {
                        color: "transparent"
                        height: 80
                    }

                    Button {
                        anchors.centerIn: parent
                        text: qsTr("选择视频")
                        onClicked: {
                            addVideo();
                        }
                    }
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
            return path.substring(8);
        }else{
            return "";
        }
    }
    FileDialog {
        id: id_pickVideoFileDialog
        title: qsTr("请选择一个视频文件")
        folder: shortcuts.home
        selectMultiple:false
        nameFilters: [ "Image files (*.mp4 *.avi)" ]
        onAccepted: {console.log(id_videoPathLabel.width,id_videoPathLabel.height)
            videoPath = shiftAbsolutePath(id_pickVideoFileDialog.fileUrl.toString());
        }
    }
    Component.onCompleted: {
    console.log("arwidth.width= ",arwidth.width, arwidth.height)
    console.log("column1.width= ",column1.width, column1.height)
    console.log("column2.width= ",column2.width, column2.height)
    console.log("row1.width= ",row1.width, row1.height)
    console.log("row2.width= ",row2.width, row2.height)
        console.log("row3.width= ",row3.width, row3.height)
    console.log(id_videoPathLabel.width,id_videoPathLabel.height)
    }
}
