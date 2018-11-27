import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
Item {
    Row {
        spacing: 20
        anchors.fill: parent
        width: parent.width
        height: parent.height
        Column {
            GroupBox {
//                border.width: 2
//                border.color: "red"

                Row {
                    Label{
                        id:selectfile_id
                        text: qsTr("选择升级文件: ")
        //                color: "red"
                    }
                    TextEdit {
                        id: textEdit
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
        Column {
            Row {
                Label{
                    id:selectfile_id1
                    text: qsTr("选择升级文件: ")
    //                color: "red"
                }
                TextEdit {
                    id: textEdit1
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
        Column {
            Row {
                Label{
                    id:selectfile_id2
                    text: qsTr("选择升级文件: ")
    //                color: "red"
                }
                TextEdit {
                    id: textEdit2
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
