import QtQuick 2.7
//import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls.Styles 1.3
import QtQuick.Controls 1.3

Rectangle {
    property string str:""
    property alias text: ll.style
//    width: 6
//    height: 6
    CheckBox
    {
        id: ll
        style:CheckBoxStyle{
            label: Label {
                id: cBoxlabel
                text: qsTr("重置为默认配置")
//                font.pixelSize: langrageSize - 5
                font.family: "KONE Information_v12"
            }
        }
    }
}
