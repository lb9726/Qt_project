import QtQuick 2.0

Rectangle {
    property alias idMtest: mtest
    width: 500
    height: 500
    MainForm {
        anchors.fill: parent
        mouseArea.onClicked: {
            Qt.quit();
        }
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            idtt3.changeColor(); // idtt3 在TestId3.qml 中alias了 property alias idtt3: tt3，但是不能全局使用
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
    TestId3 {
        x: 240
        y: 80
        width: 40
        height: 40
    }
}

