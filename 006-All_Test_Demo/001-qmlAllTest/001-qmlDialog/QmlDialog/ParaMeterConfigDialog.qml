import QtQuick 2.7
import QtQuick.Dialogs 1.2

Dialog {
    id: idD
    title: qsTr("参数配置")
    modality : Qt.WindowModal
    width: 800
    height: 800
    Rectangle {
        color: "transparent"
        width: parent.width
        height: parent.height
    }
    onAccepted: {
        console.log("is onAccepted");
    }
    onRejected: {
        console.log("is onRejected");
    }
}
