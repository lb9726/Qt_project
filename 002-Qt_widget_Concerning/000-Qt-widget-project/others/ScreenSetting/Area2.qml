import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Item {
    ColumnLayout {
        RowLayout {
            CheckBox {
                id: id_dateTimerChecker
                text: qsTr("显示时间和日期")
            }
        }
        RowLayout {
            spacing: 30
            enabled: id_dateTimerChecker.checked
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
            enabled: id_dateTimerChecker.checked
            spacing: 30
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
                id: id_standbyChecker
                text: qsTr("屏保")
            }
        }
        RowLayout {
            enabled: id_standbyChecker.checked
            spacing: 40
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
            enabled: id_standbyChecker.checked
            spacing: 8
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
                editable: true
            }
        }
        RowLayout {
            enabled: id_standbyChecker.checked
            spacing: 40
            Label {
                text: qsTr("二阶段周期")
            }
//            SpinBox {
//                from: 0
//                to: 3000
//                value: 1200
//            }
            ComboBox {

            }
        }
        RowLayout {
            enabled: id_standbyChecker.checked
            spacing: 8
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
