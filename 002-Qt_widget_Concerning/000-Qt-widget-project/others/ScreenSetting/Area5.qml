import QtQuick 2.7
//import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls.Styles 1.3
import QtQuick.Controls 1.3
Item {
    property bool resetChecked: false
    property bool confirmedOnce: false
    onResetCheckedChanged: {
        if (resetChecked)
        {
            reset_default.checked = true
            console.log("resetchecked = ", resetChecked)
            id_resetWarnningBox.open();
        }
        else
        {
            reset_default.checked = false
        }
    }

    Item {
        width: parent.width
        height: 20
        id: updatedisk_id
        Label {
            text: qsTr("升级盘制作")
            anchors.centerIn: parent
        }
    }
    ColumnLayout {
        spacing: 20
        RowLayout {

        }
        RowLayout {            
            CheckBox {
                id: reset_default
                text: qsTr("重置为默认配置")
                onCheckedChanged: {
                    if (checked)
                    {
                        console.log("checked = ", checked)
                        id_resetWarnningBox.open();
                    }
                }
            }
        }
        RowLayout {
            spacing: 50
            Text {
                id: make_update_disk
                text: qsTr("制作升级盘")
            }
            ComboBox {

            }
            Button {
                id: btn
                text: qsTr("制作")
                enabled: false
//                background: BorderImage {
//                    id: name
//                    source: btn.pressed ? "qrc:///Brown.png":"qrc:///Blue.png"
//                    width: btn.width; height: btn.height
//                    border.left: 5; border.top: 5
//                    border.right: 5; border.bottom: 5
//                }
                style: ButtonStyle {
                    background: BorderImage {
                        source: btn.pressed ? "qrc:///Brown.png":"qrc:///Blue.png"
                        width: btn.width; height: btn.height
                        border.left: 5; border.top: 5
                        border.right: 5; border.bottom: 5
                    }
                    label:Rectangle {
                        color: "transparent"
                        Text{
//                            width: btn.width; height:btn.height
                            text: btn.text
                            color:"white"
                            anchors.centerIn: parent
//                            font.family: qsTr("微软雅黑")
//                            font.pixelSize:12
//                            maximumLineCount:1   //设置最大最行数
//                            elide:Text.ElideLeft
//                            wrapMode:Text.Wrap
                        }
                    }
                }

            }
        }
    }
//    Popup {
//        id: id_resetWarnningBox
////        x:(updatedisk_id.width - width)/2
////        y:(updatedisk_id.height - height)/2
//        width: 100
//        height: 60
//        modal: true
//        focus: true
//        closePolicy: Popup.NoAutoClose
//        Item{
//            width: 600
//            height: 300
//            anchors.centerIn: parent
//            Text {
//                id: id_resetContent
//                width: 500
//                height: 200
//                anchors.top: parent.top
//                anchors.topMargin: 50
//                anchors.horizontalCenter: parent.horizontalCenter
//                text: qsTr("打开此开关选项,屏幕将恢复默认的参数配置.多媒体,滚动字幕,标题,时间和日期,系统配置参数都将恢复为默认.")
//                wrapMode:Text.WrapAnywhere
//            }
//            RowLayout{
//                anchors.horizontalCenter: parent.horizontalCenter
//                anchors.bottom: parent.bottom
//                anchors.bottomMargin: 10
//                height: 50
//                spacing: 50
//                Button{
//                    text:qsTr("确认")
//                    onClicked:{
//                        resetChecked = true
//                        id_resetWarnningBox.close();
//                        //}
//                    }
//                }
//                Button{
//                    text:qsTr("取消")
//                    onClicked:
//                    {
//                        id_resetWarnningBox.close();
//                        reset_default.checked = false
//                    }
//                }
//            }
//        }
//    }
    Component.onCompleted: {
        //console.log("make12222");
        UsbHelper.scanHD();
        //console.log("make11");
    }
}
