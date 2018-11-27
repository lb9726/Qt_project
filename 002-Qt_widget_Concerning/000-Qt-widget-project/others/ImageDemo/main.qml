import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 2.1
//import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
ApplicationWindow {
    id: id_mediaScreenHelper
    visible: true
    width: 1240
    height: 998
    title: qsTr("Hello World")

//    Horizontal_Image
//    {
//        id: open_view
//        visible: false
//    }
    Vertical_Image {
        id: open_vertical_view
    }
    Horizontal_Image {
        id: open_view
        scale: 1
    }

    Button {
        x: 10
        y: 10
        width: 20
        height: 10
        onClicked: {
            open_view.open();
        }
    }
    Button_Circle {
        x: parent.width - 20;

    }
}
