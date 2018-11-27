import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.2

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
}
