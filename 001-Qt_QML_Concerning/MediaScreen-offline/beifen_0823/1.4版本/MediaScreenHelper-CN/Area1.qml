import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Item {
    ColumnLayout {
        spacing: 10
        RowLayout{
            CheckBox {
                id: id_briVolumeChecker
                text: qsTr("亮度&音量")
            }
        }
        RowLayout {
            enabled: id_briVolumeChecker.checked
            spacing: 10
            Label {
                text: qsTr("亮度")
            }
            Slider {
                id: slider_bright
                width: 50
                from:0
                to:100
                value: 0
                onValueChanged: {
                    spinbox_bright.value = value
                }
            }
            SpinBox {
                id: spinbox_bright
                from: 0
                to: 100
                value: 0
                onValueChanged: {
                    slider_bright.value = value
                }
            }
        }
        RowLayout {
            enabled: id_briVolumeChecker.checked
            spacing: 10
            Label {
                text: qsTr("音量")
            }
            Slider {
                id:slider_volume
                width: 50
                from:0
                to:100
                value: 0
                onValueChanged: {
                    spinbox_volume.value = value
                }
            }
            SpinBox {
                id: spinbox_volume
                width: 50
                from: 0
                to: 100
                value: 0
                onValueChanged: {
                    slider_volume.value = value
                }
            }
        }
        RowLayout {
            Label {
                text: qsTr("滚动文字")
            }
        }
        RowLayout {
            CheckBox {
                id: id_scrollintTextChecker
                text: qsTr("隐藏滚动文字")
                onCheckedChanged: {
                    if (checked)
                    {
                        id_scrollintTextChecker.text = qsTr("显示滚动文字")
                    }
                    else
                    {
                       id_scrollintTextChecker.text = qsTr("隐藏滚动文字")
                    }
                }
            }
        }
        RowLayout {
            Item {
                width: 20
            }
            TextField {
                visible:id_scrollintTextChecker.checked
                id: scrolltext
                placeholderText: qsTr("请输入字幕文字")
            }
        }
        RowLayout {
            Label {
                id: title_id
                text: qsTr("标题")
            }
        }
        RowLayout {
            CheckBox {
                id: id_titleChecker
                text: qsTr("隐藏标题")
                onCheckedChanged: {
                    if (checked)  // 多国语言翻译的时候需要修改
                    {
                        id_titleChecker.text = qsTr("显示标题")
                    }
                    else
                    {
                        id_titleChecker.text = qsTr("隐藏标题")
                    }
                }
            }
        }
        RowLayout {
            visible: id_titleChecker.checked
            Item {
                width: 40
            }
            TextField {
                id: titletext
                placeholderText: qsTr("请输入标题文字")
                anchors.centerIn: parent
            }
        }
    }

}
