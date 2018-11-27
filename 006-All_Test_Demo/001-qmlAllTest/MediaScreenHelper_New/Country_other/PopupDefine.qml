import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Popup {
    property int windowsWidth: 1440
    property int windowsHeight: 740
    id: id_ColseWarnningBox
    x: (windowsWidth - width)/2
    y: (windowsHeight - height)/2
    width: 600
    height: 300
    modal: true
    focus: true
    closePolicy: Popup.NoAutoClose
    Item {
        width: 600
        height: 300
        anchors.centerIn: parent
        Text {
            id: id_ColseContent
            width: parent.width
            height: parent.height
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: qsTr("点击确认，将重新加载界面布局，Usb设备需要重新插拔!")
            wrapMode:Text.WrapAnywhere
        }
        RowLayout {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            height: 50
            spacing: 50
            Item {
                width: parent.width
                ButtonSelfDefine {
                    id: mainColse_btn_OK
                    btext: qsTr("确认")
                    x: 50
                    bwidth: 66
                    bheight: 36
                    onButtonClick: {
                        id_resetWarnningBox.close();
                        id_mediaScreenHelper.close();
                        MediaScreen.setIndex(3) // 设置切换布局的索引值
                        MediaScreen.changeUi(); // 切换俄语布局界面
                        MediaScreen.sendUdiskInformation(); // 切换界面显示Usb
                    }
                }
                ButtonSelfDefine {
                    id: mainColse_btn_Cancel
                    x: parent.width - 50 - bwidth
                    btext: qsTr("取消")
                    bwidth: 66
                    bheight: 36
                    onButtonClick: {
                        are4_languageChanges.ischeckedchange();
                        id_ColseWarnningBox.close();
                    }
                }
            }
        }
    }
}
