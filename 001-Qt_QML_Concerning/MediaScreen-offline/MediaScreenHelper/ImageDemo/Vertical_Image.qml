import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls.Material 2.0

Popup {
    property string vborder_Color: "red";
    property string hborder_Color: "grey";
//    property bool vBtn_enable: false;
//    property bool hBtn_enable: true;
    id: id_resetWarnningBox
    x:(id_mediaScreenHelper.width - width)/2
    y:(id_mediaScreenHelper.height - height)/2
    width: 600
    height: 687
    modal: true
    focus: true
    closePolicy: Popup.NoAutoClose
    Button_Circle {
        x: parent.width - 20
        y: -5
        onButtonclick: {
            open_vertical_view.close();
        }
    }
    ColumnLayout {
        anchors.fill: parent
        spacing: 15
        RowLayout {
            Text {
                id: name
                text: qsTr("布局预览")
            }
        }
        RowLayout {
            Rectangle {
                id: greyline
                x: -15
                width: 585
                height: 2
                color: "grey"
            }
        }

        RowLayout {
            width: parent.width
            spacing: 10
            Rectangle {
                width: 35
                height: 20
                color: "white"
            }

            Rectangle {
                id: hRect
                width:btn_horizatal.width + 4
                height: btn_horizatal.height + 4
                border.color: hborder_Color
                border.width: 1
                Button_Self {
                    id: btn_horizatal
                    anchors.centerIn: parent
                    checkable: true
//                    checked: true;
                    text: qsTr("横屏显示")
                    onClicked:
                    {
                        hborder_Color = "red";
                        vborder_Color = "grey"
                        open_vertical_view.close();
                        open_view.open();
                    }
                }
            }
            Rectangle {
                id: vRect
                width:btn_vertical.width + 4
                height: btn_vertical.height + 4
                border.color: vborder_Color
                border.width: 1
                Button_Self {
                    id: btn_vertical
                    anchors.centerIn: parent
                    checkable: true;
                    checked: true;
                    enabled: false
                    text: qsTr("竖屏显示")
                    onClicked:
                    {
                        vborder_Color = "red";
                        hborder_Color = "grey";
                        open_view.close();
                        open_vertical_view.open();
                    }
                }
            }
        }
        RowLayout {
            width: parent.width
            Rectangle {
                width: 90
                height: 20
                color: "white"
            }
//            Item{
//                width: 512
//                height: 384
//                anchors.fill: parent
//                Image {
//                    x: 106
//                    y: 140
//                    id: image_back
//                    width: 391
//                    height: 520
//                    anchors.centerIn: parent
//                    source: "qrc:///images/iot15_v.png"
//                }
//                Image {
//                    x: 106 - 46
//                    y: 340 - 212
//                    width: 391
//                    height: 294
//                    source: "qrc:///images/Lighthouse.jpg"
//                }
                New_Vertical
                {

                }
//            }
        }
    }
}
