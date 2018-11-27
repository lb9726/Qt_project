import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3

Item {
    width: 1000
    height: 650
    RowLayout {
        x: 50; y: 35
        spacing: 20
        ColumnLayout {
            Rectangle {
                color: "transparent"
                width: 200
                height: 500
                border.color: "red"
                border.width: 1
                visible: true
                RowLayout {
                    Label{
                        //                id:selectfile_id2
                        text: qsTr("选择升级文件: ")
                        //                color: "red"
                    }
                    TextEdit {
                        //                id: textEdit2
                        text: qsTr("Enter some text...")
                        verticalAlignment: Text.AlignVCenter
                        Rectangle {
                            anchors.fill: parent
                            //                        anchors.margins: -10
                            color: "transparent"
                            border.width: 1
                        }
                    }
                }
            }
        }
        ColumnLayout {
            Layout.alignment: Qt.AlignTop
            Rectangle {
                visible: true
                RowLayout {
                    Label{
                        //                id:selectfile_id2
                        text: qsTr("选择升级文件: ")
                        //                color: "red"
                    }
                    TextEdit {
                        //                id: textEdit2
                        text: qsTr("Enter some text...")
                        verticalAlignment: Text.AlignVCenter
                        Rectangle {
                            anchors.fill: parent
                            //                        anchors.margins: -10
                            color: "transparent"
                            border.width: 1
                        }
                    }
                }
            }
        }
        ColumnLayout {
            Layout.alignment: Qt.AlignTop
            Rectangle {
                visible: true
                RowLayout {
                    Label{
                        //                id:selectfile_id2
                        text: qsTr("选择升级文件: ")
                        //                color: "red"
                    }
                    TextEdit {
                        //                id: textEdit2
                        text: qsTr("Enter some text...")
                        verticalAlignment: Text.AlignVCenter
                        Rectangle {
                            anchors.fill: parent
                            //                        anchors.margins: -10
                            color: "transparent"
                            border.width: 1
                        }
                    }
                }
            }
        }
    }
}

