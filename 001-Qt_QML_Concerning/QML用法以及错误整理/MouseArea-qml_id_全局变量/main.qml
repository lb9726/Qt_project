import QtQuick 2.4
import QtQuick.Window 2.2

Window {
    property alias idMtest: mtest
    visible: true
    width: 500
    height: 500
    MainForm {
        anchors.fill: parent
        mouseArea.onClicked: {
            Qt.quit();
        }        
    }
    TestAlias {
        id: mtest
        x: 50
        y: 80
        width: 80
        height: 80
    }
    Test2 {
        x: 140
        y: 80
        width: 40
        height: 40
    }
}
