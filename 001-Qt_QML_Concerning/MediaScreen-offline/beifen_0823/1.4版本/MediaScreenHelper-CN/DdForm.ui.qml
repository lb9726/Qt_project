import QtQuick 2.4
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Item {
    width: 800
    height: 500

    RowLayout {
        id: rowLayout
        x: -22
        y: 200
        width: 357
        height: 100

        ColumnLayout {
            id: columnLayout
            width: 43
            height: 42

            Label {
                id: label1
                width: 55
                height: 12
                text: qsTr("Label")
            }

            Label {
                id: label
                width: 55
                height: 12
                text: qsTr("Label")
            }
        }

        ColumnLayout {
            id: columnLayout1
            width: 43
            height: 42

            Slider {
                id: slider1
                value: 0.5
            }

            Slider {
                id: slider
                value: 0.5
            }
        }

        ColumnLayout {
            id: columnLayout2
            width: 43
            height: 42
            Layout.maximumHeight: 100
            Layout.maximumWidth: 100

            SpinBox {
                id: spinBox
                width: 92
            }

            SpinBox {
                id: spinBox1
                width: 92
            }
        }
    }

    RowLayout {
        id: rowLayout1
        x: -7
        y: 322
        width: 366
        height: 53

        Label {
            id: label2
            text: qsTr("Label")
        }

        TextField {
            id: textField
            text: qsTr("Text Field")
        }

        CheckBox {
            id: checkBox
            text: qsTr("Show Scrolling Text")
        }
    }

    RowLayout {
        id: rowLayout2
        x: -7
        y: 381
        width: 366
        height: 53

        Label {
            id: label3
            text: qsTr("Label")
        }

        TextField {
            id: textField1
            text: qsTr("Text Field")
        }
        CheckBox {
            id: checkBox1
            text: qsTr("Show Title")
        }
    }

    ColumnLayout {
        id: columnLayout3
        x: 426
        y: 8
        width: 366
        height: 190

        CheckBox {
            id: checkBox2
            text: qsTr("Check Box")
        }

        RowLayout {
            id: rowLayout3
            width: 100
            height: 100

            ColumnLayout {
                id: columnLayout4
                width: 100
                height: 100

                Label {
                    id: label4
                    text: qsTr("Label")
                }

                RadioButton {
                    id: radioButton
                    text: qsTr("Radio Button")
                }

                RadioButton {
                    id: radioButton1
                    text: qsTr("Radio Button")
                }
            }

            ColumnLayout {
                id: columnLayout5
                width: 100
                height: 100

                Label {
                    id: label5
                    text: qsTr("Label")
                }

                RadioButton {
                    id: radioButton2
                    text: qsTr("Radio Button")
                }

                RadioButton {
                    id: radioButton3
                    text: qsTr("Radio Button")
                }
            }
        }
    }

    ColumnLayout {
        id: columnLayout6
        x: 396
        y: 200
        width: 366
        height: 249


        Label {
            id: label6
            text: qsTr("Label")
        }
        RowLayout {
            id: rowLayout4
            width: 100
            height: 100

            Label {
                id: label7
                text: qsTr("Label")
            }

            Slider {
                id: slider2
                value: 0.5
            }

            SpinBox {
                id: spinBox2
                width: 92
            }
        }

        RowLayout {
            id: rowLayout5
            width: 100
            height: 100
            Label {
                id: label8
                text: qsTr("Label")
            }

            Slider {
                id: slider3
                value: 0.5
            }

            SpinBox {
                id: spinBox3
                width: 92
            }
        }
    }
}
