//import QtQuick 2.7
//import QtQuick.Controls 2.1
//import QtQuick.Layouts 1.3
import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
Item {
    RowLayout {
//        width: 873
//        height: 552
        ColumnLayout {
            id:lightandvolume_groupbox
            anchors.top: parent.top
//            spacing: 20
            Rectangle {
                width: 287
                height: 307
                color: "transparent"
                border.color: "red"
                border.width: 1
                ColumnLayout {
                    RowLayout{
                        Label {
                            id: lightandvolume
                            text: qsTr("亮度&音量")
                        }
                    }
                    RowLayout {
                        spacing: 10
                        Label {
                            text: qsTr("亮度")
                        }
                        Slider {
                            width: 107
                            from:0
                            to:100
                            value: 0
                        }
                        SpinBox {
                            from: 0
                            to: 100
                            value: 0
                        }
                    }
                    RowLayout {
                        spacing: 10
                        Label {
                            text: qsTr("音量")
                        }
                        Slider {
                            width: 107
                            from:0
                            to:100
                            value: 0
                        }
                        SpinBox {
                            width: 50
                            from: 0
                            to: 100
                            value: 0
                        }
                    }
                    RowLayout {
                        Label {
                            text: qsTr("滚动文字")
                        }
                    }
                    RowLayout {
                        CheckBox {
                            text: qsTr("显示滚动文字")
                        }
                    }
                    RowLayout {
                        TextField {
                            placeholderText: qsTr("请输入字幕文字")
                        }
                    }
                    RowLayout {
                        Label {
                            text: qsTr("标题")
                        }
                    }
                    RowLayout {
                        CheckBox {
                            text: qsTr("显示标题")
                        }
                    }
                    RowLayout {
                        TextField {
                            placeholderText: qsTr("请输入标题文字")
                        }
                    }
                }
            }
        }
        ColumnLayout {
            id:timeanddate_groupbox
            anchors.top: parent.top
            anchors.bottom: lightandvolume_groupbox.bottom
            Rectangle {
                width: 369
                height: 307
                color: "transparent"
                border.color: "red"
                border.width: 1
                ColumnLayout {
                    RowLayout {
                        CheckBox {
                            text: qsTr("显示时间和日期")
                        }
                    }
                    RowLayout {
                        Label {
                            text: qsTr("时间格式")
                        }
                        RadioButton {
                            text: qsTr("12上午下午")
                        }
                        RadioButton {
                            text: qsTr("24小时")
                        }
                    }
                    RowLayout {
                        Label {
                            text: qsTr("日期格式")
                        }
                        RadioButton {
                            text: qsTr("yyyy:mm:dd")
                        }
                        RadioButton {
                            text: qsTr("mm:dd:yyyy")
                        }
                    }
                    RowLayout {
                        CheckBox {
                            text: qsTr("屏保")
                        }
                    }
                    RowLayout {
                        Label {
                            text: qsTr("一阶段周期")
                        }
                        SpinBox {
                            from: 0
                            to: 100
                            value: 0
                        }
                    }
                    RowLayout {
                        Label {
                            text: qsTr("亮度")
                        }
                        Slider {
                            from: 0
                            to: 100
                            value: 0
                        }
                        SpinBox {
                            from: 0
                            to: 100
                            value: 0
                        }
                    }
                    RowLayout {
                        Label {
                            text: qsTr("二阶段周期")
                        }
                        SpinBox {
                            from: 0
                            to: 3000
                            value: 1200
                        }
                    }
                    RowLayout {
                        Label {
                            text: qsTr("亮度")
                        }
                        Slider {
                            from: 0
                            to: 100
                            value: 0
                        }
                        SpinBox {
                            from: 0
                            to: 100
                            value: 0
                        }
                    }
                }
            }
        }
        ColumnLayout {
            id:updatemultimedia_groupbox
            anchors.top: parent.top
            anchors.bottom: lightandvolume_groupbox.bottom
            Rectangle {
                width: 173
                height: 307
                color: "transparent"
                border.color: "red"
                border.width: 1
                ColumnLayout {
                    RowLayout {
                        Label {
                            text: qsTr("多媒体更新")
                        }
                    }
                    RowLayout {
                        RadioButton {
                            text: qsTr("正常显示")
                        }
                    }
                    RowLayout {
                        RadioButton {
                            text: qsTr("全屏显示")
                        }
                    }
                    RowLayout {
                        RadioButton {
                            text: qsTr("视频")
                        }
                        RadioButton {
                            text: qsTr("图片")
                        }
                    }
                    RowLayout {
                        TextArea {

                        }
                    }
                }
            }
        }
    }
    RowLayout {
        ColumnLayout {

        }
        ColumnLayout {

        }
    }
}
