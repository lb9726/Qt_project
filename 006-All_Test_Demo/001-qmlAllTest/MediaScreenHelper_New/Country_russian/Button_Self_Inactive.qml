import QtQuick 2.7
//import QtQuick.Controls 2.0
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.0
import QtQuick.Controls.Styles 1.3

Button {
    id: rBtn
    property alias btext: rBtn.text
    property alias bwidth: rBtn.width
    property alias bheight: rBtn.height
    clip: true
    signal buttonClick();
    style: ButtonStyle {
        background: BorderImage {
            source: rBtn.pressed ? "qrc:///image/GreyShen.png" : "qrc:///image/GreyQian.png"
            width:  280;
            height: rBtn.height
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
    onClicked: {
        buttonClick();
    }
}
