import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
Item {
    ColumnLayout
    {
        Rectangle {
//            color: "transparent"
            color: "blue"
            border.color: "black"
            border.width: 2
            anchors.fill: parent
            width: 150
            height: 650
            visible: true
            RowLayout {
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: 20
                spacing: 20
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
//    ColumnLayout {

//    }
//    ColumnLayout {

//    }
    Rectangle {

    }
    Rectangle {

    }
    Rectangle {

    }
}
