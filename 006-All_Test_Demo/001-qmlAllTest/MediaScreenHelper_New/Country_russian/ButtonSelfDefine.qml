import QtQuick 2.7
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.0
import QtQuick.Controls.Styles 1.3

Button {
    property alias btext: select_picture_btn.text
    property alias bwidth: select_picture_btn.width
    property alias bheight: select_picture_btn.height
    id: select_picture_btn
    clip: true
    signal buttonClick();
    style: ButtonStyle {
        background: BorderImage {
            source: select_picture_btn.pressed ? "qrc:///image/Brown_464.png" : "qrc:///image/Blue_464.png"
            width: select_picture_btn.width;
            height: select_picture_btn.height
            border.left: 5; border.top: 5
            border.right: 5; border.bottom: 5
        }
        label:Rectangle {
            color: "transparent"
            Text{
                text: select_picture_btn.text
                color: "white"
                anchors.centerIn: parent
                font.family: "KONE Information_v12"
            }
        }
    }
    onClicked: {
        console.log("ButtonSelfDefine.qml is clicked");
        buttonClick();
    }
}

