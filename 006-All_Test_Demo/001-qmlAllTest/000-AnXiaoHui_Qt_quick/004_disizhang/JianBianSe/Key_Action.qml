import QtQuick 2.0

Rectangle {
    width: 400
    height: 400
    color: "#c0c0c0"
    focus: true
    Keys.enabled: true
    Keys.onBackPressed: Qt.quit()
    Keys.onEscapePressed: Qt.quit()
    Keys.onPressed: {
        switch(event.key)
        {
        case Qt.Key_0:
        case Qt.Key_1:
        case Qt.Key_2:
        case Qt.Key_3:
        case Qt.Key_4:
        case Qt.Key_5:
        case Qt.Key_6:
        case Qt.Key_7:
        case Qt.Key_8:
        case Qt.Key_9:
            event.accepted = true; //设置成了事件已接收，防止向上层传递
            keyView.text = event.key - Qt.Key_0;
            console.log(event.key);
            break;
        default:
            event.accepted = true;
            console.log(event.key);
            break;
        }
    }
    Text {
        id: keyView
        font.bold: true
        font.pixelSize: 24
        text: qsTr("KeyBoardInsert")
        anchors.centerIn: parent
    }
}
