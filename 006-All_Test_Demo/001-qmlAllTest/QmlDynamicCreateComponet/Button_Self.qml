import QtQuick 2.7
//import QtQuick.Controls 2.0
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.0
import QtQuick.Controls.Styles 1.3

Button {
    id: rBtn
    style: ButtonStyle {
        background: BorderImage {
            source: rBtn.pressed ? "qrc:///image/Brown_238_41.png":"qrc:///image/Blue_238_41.png"
            width: rBtn.width; height: rBtn.height
            border.left: 5; border.top: 5
            border.right: 5; border.bottom: 5
        }
        label:Rectangle {
            color: "transparent"
            Text{
                text: rBtn.text
                anchors.centerIn: parent
                font.family: "KONE Information_v12"
            }
        }
    }
}
