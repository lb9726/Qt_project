import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

ApplicationWindow {
    title: qsTr("Hello World")
    width: 1024
    height: 768
    visible: true

    Image {
        id: background
        fillMode: Image.TileHorizontally
        smooth: true
        source: 'images/background.png'
    }

    Rollingtext {
        x: 290
        y: 684
    }
}
