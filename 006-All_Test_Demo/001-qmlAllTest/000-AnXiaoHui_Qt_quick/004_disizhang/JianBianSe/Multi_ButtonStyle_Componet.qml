import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

Rectangle {
    width: 200
    height: 200

    Component {
        id: btnStyle;
        ButtonStyle {
            background: Rectangle {
                implicitWidth: 70
                implicitHeight: 25
                color: "#DDDDDD"
                border.width: control.pressed ? 2 : 1
                border.color: (control.hovered || control.pressed) ? "green" : "#888888"
            }
        }
    }

    Button {
        text: "A"
        style: btnStyle
    }
    Button {
        y: 100
        text: "B"
        style: btnStyle
    }
}
