import QtQuick 2.7
//import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls.Styles 1.3
import QtQuick.Controls 1.3

Item {
    ColumnLayout {
        RowLayout {
            CheckBox {
                id: id_dateTimerChecker
                text: qsTr("显示时间和日期")
//                font.family: "KONE Information_v12"
            }
        }
        RowLayout {
            spacing: 30
            enabled: id_dateTimerChecker.checked
            Label {
                text: qsTr("时间格式")
                font.family: "KONE Information_v12"
            }
            RadioButton {
                text: qsTr("12上午下午")
//                font.family: "KONE Information_v12"
            }
            RadioButton {
                checked: true
                text: qsTr("24小时")
//                Font.family: "KONE Information_v12"
            }
        }
        RowLayout {
            enabled: id_dateTimerChecker.checked
            spacing: 30
            Label {
                text: qsTr("日期格式")
                font.family: "KONE Information_v12"
            }
            RadioButton {
                checked: true
                text: qsTr("yyyy:mm:dd")
            }
            RadioButton {
                text: qsTr("mm:dd:yyyy")
            }
        }
        RowLayout {
            CheckBox {
                id: id_standbyChecker
                text: qsTr("屏保")
            }
        }
        RowLayout {
            enabled: id_standbyChecker.checked
            spacing: 40
            Label {
                id: label_stage1
                text: qsTr("一阶段周期(S)")
            }
            SpinBox {
                minimumValue: 0
                maximumValue : 100
                value: 0
            }
        }
        RowLayout {
            enabled: id_standbyChecker.checked
            spacing: 8
            Label {
                id: label_bright_stage1
                text: qsTr("亮度")
            }
            Slider {
                id: slider_bright_stage1
                minimumValue: 0
                maximumValue : 100
                value: 0
                onValueChanged: {
                    spinbox_bright_stage1.value = value
                }

            }
            SpinBox {
                id: spinbox_bright_stage1
                minimumValue: 0
                maximumValue : 100
                value: 0
                onValueChanged: {
                    slider_bright_stage1.value = value
                }
            }
        }
        RowLayout {
            enabled: id_standbyChecker.checked
            spacing: 40
            Label {
                id: label_stage2
                text: qsTr("二阶段周期(S)")
            }
            SpinBox {
                minimumValue: 0
                maximumValue : 3000
                value: 1200
            }
        }
        RowLayout {
            enabled: id_standbyChecker.checked
            spacing: 8
            Label {
                id: label_bright_stage2
                text: qsTr("亮度")
            }
            Slider {
                id: slider_bright_stage2
                minimumValue: 0
                maximumValue : 100
                value: 0
                onValueChanged: {
                    spinbox_bright_stage2.value = value
                }
            }
            SpinBox {
                id: spinbox_bright_stage2
                minimumValue: 0
                maximumValue : 100
                value: 0
                onValueChanged: {
                    slider_bright_stage2.value = value
                }
            }
        }
    }

}
