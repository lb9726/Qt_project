import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.2

Item {
    id: root
    //anchors.fill: parent
    Button {
        width: windowWidth/20 > 50 ? 50: windowWidth/20
        height: windowWidth/20 > 50 ? 50: windowWidth/20
        opacity: pressed ? themeOpacity : 1

        anchors {
            left: parent.left
            top: parent.top
            margins: windowWidth > windowHeight  ? windowHeight/20 : windowWidth/20
        }

        style: ButtonStyle {
            background: Image {
                source: "qrc: /images/back.png"
            }

        }

        onClicked: {
           uiLogicalEngine(4, 1)
        }
    }

    ExclusiveGroup { id: template_RadioGroup }

    Rectangle {
        id: device_select_area
        width: parent.width*3/4
        height: parent.height/2
        anchors.centerIn: parent
        anchors.verticalCenterOffset: -parent.height/8
        color: "transparent"
        Row {
            anchors.fill: parent
            spacing: 20
            Column {
                width: parent.width/2
                height: parent.height
                Item {
                    width: parent.width
                    height: parent.height/2
                    //color: "red"
                    RadioButton {
                        anchors.centerIn: parent
                        text: "从当前设备获取"
                        width: parent.width/2
                        height: parent.height/4
                        exclusiveGroup: template_RadioGroup
                        checked: true
                        style: RadioButtonStyle {
                            indicator: Rectangle {
                                implicitWidth: control.height
                                implicitHeight: control.height
                                radius: control.height/2
                                border.color: "white";
                                border.width: 1
                                color: "transparent";
                                Rectangle {
                                    anchors.fill: parent
                                    visible: control.checked
                                    color: "white";
                                    radius: control.height/2
                                    anchors.margins: control.height/5
                                }
                            }
                            label: Text {
                                color: "white";
                                text: control.text
                                font.bold: true
                                font.pixelSize: control.height/2
                            }
                            spacing: control.width/8
                        }
                    }
                }
                Item {
                    width: parent.width
                    height: parent.height/2

                    Rectangle {
                        width: parent.width
                        height: parent.height/2
                        color: viceColor
                        anchors.centerIn: parent
                        //opacity: 0.3
                        Text {
                            text: "获取到本地"
                            anchors.centerIn: parent
                            color: "white"
                            font.pixelSize: parent.height*3/8
                        }
                        MouseArea {
                            anchors.fill: parent
                            //onClicked: sig_online();
                            onPressed: parent.opacity = themeOpacity;
                            onReleased: parent.opacity = 1;
                        }
                    }
                }
            }

            Column {
                width: parent.width/2
                height: parent.height
                Item {
                    width: parent.width*3/4
                    height: parent.height/2
                    //color: "red"
                    RadioButton {
                        anchors.centerIn: parent
                        text: "从模板新建"
                        width: parent.width/2
                        height: parent.height/4
                        exclusiveGroup: template_RadioGroup
                        style: RadioButtonStyle {
                            indicator: Rectangle {
                                implicitWidth: control.height
                                implicitHeight: control.height
                                radius: control.height/2
                                border.color: "white";
                                border.width: 1
                                color: "transparent";
                                Rectangle {
                                    anchors.fill: parent
                                    visible: control.checked
                                    color: "white";
                                    radius: control.height/2
                                    anchors.margins: control.height/5
                                }
                            }
                            label: Text {
                                color: "white";
                                text: control.text
                                font.bold: true
                                font.pixelSize: control.height/2
                            }
                            spacing: control.width/8
                        }
                    }
                }
                Item {
                    width: parent.width
                    height: parent.height/2
                    //color: "red"
                    Row {
                        height: parent.height/2
                        width: parent.width
                        anchors.centerIn: parent
                        Rectangle {
                            height: parent.height
                            width: parent.width*4/5
                            color: "white"
                            Text {
                                id: template_name
                                anchors.fill: parent
                                text: "选择设备型号"
                                verticalAlignment: Text.AlignVCenter
                                font.pixelSize: height/2
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    template_selectLoader.sourceComponent = template_list
                                }
                            }
                        }
                        Rectangle {
                            height: parent.height
                            width: parent.width/5
                            color: "white"
                            Image {
                                height: parent.height/2
                                width: parent.width/2
                                source: "qrc: /images/down_Arror.png"
                                anchors.centerIn: parent
                                antialiasing: true
                            }
                        }
                    }
                }
            }
        }
    }

    Rectangle {
        color: viceColor
        anchors {
            top: device_select_area.bottom
            topMargin: parent.height/8
            horizontalCenter: device_select_area.horizontalCenter
        }
        width: device_select_area.width/3
        height: parent.height/8
        Text {
            text: "确定"
            anchors.centerIn: parent
            color: "white"
            font.pixelSize: parent.height/2
        }
        MouseArea {
            anchors.fill: parent
            onClicked: uiLogicalEngine(4, 0)
            onPressed: parent.opacity = themeOpacity;
            onReleased: parent.opacity = 1;
        }
    }

    Loader {
        id: template_selectLoader
        anchors.fill: parent
        z: 10
    }

    Component {
        id: template_list
        Item {
            anchors.fill: parent
            MouseArea {
                anchors.fill: parent
                z: -1
            }
            Item {
                id: listview_container
                width: parent.width/2
                height: parent.height*3/4
                anchors.centerIn: parent
                clip: true
                Rectangle {
                    color: viceColor
                    anchors.fill: parent
                    z: -1
                    //opacity: themeOpacity
                }

                ListView {
                    anchors.fill: parent
                    model: template_listModel
                    delegate: Item {
                        width: listview_container.width
                        height: listview_container.height/10

                        Rectangle {
                            width: parent.width
                            height: 2
                            opacity: 0.4
                            z: -1
                            anchors.bottom: parent.bottom
                        }

                        Text {
                            text: name
                            color: "white"
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            //anchors.horizontalCenterOffset: listview_container.height/4
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                template_name.text = name
                                template_selectLoader.sourceComponent = null
                            }
                        }

                    }
                }
            }

        }
    }

    ListModel {
        id: template_listModel
        ListElement {
            name: "Bill Smith"
        }
        ListElement {
            name: "John Brown"
        }
        ListElement {
            name: "Sam Wise"
        }
    }
}
