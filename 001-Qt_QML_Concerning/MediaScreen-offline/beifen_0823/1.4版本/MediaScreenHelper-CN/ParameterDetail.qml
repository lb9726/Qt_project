import QtQuick 2.0
import QtQuick.Controls 1.4
Item {
    width: 860
    height: 400
    anchors.centerIn: parent
    Row {
        id: rowLayout
        x: 0
        y: 0
        width: 430
        height: 50

        Row{
            width: parent.width
            height: 50
            Item {
                //id: id_labelBrightness
                width: 80
                height: parent.height
                Text{
                    text: qsTr("Brightness")
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
            Slider {
                id: slider1
                width: 100
                height: 40
                value: 80
            }
            Item {
                id: spinBox
                width: 10
                height: 40
                Text{
                    id:id_brightnessValue
                    text: slider1.value
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
    }

    Row {
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

    Row {
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

    Column {
        id: columnLayout3
        x: 426
        y: 8
        width: 366
        height: 190

        CheckBox {
            id: checkBox2
            text: qsTr("Check Box")
        }

        Row {
            id: rowLayout3
            width: 100
            height: 100

            Column {
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

            Column {
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

    Column{
        id: columnLayout6
        x: 396
        y: 200
        width: 366
        height: 249


        Label {
            id: label6
            text: qsTr("Label")
        }
        Row {
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

        Row{
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
