import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2
//import QtQuick.Layouts 1.1
//import QtQuick.Window 2.0

Dialog {
    id: filledDialog
    modality: Qt.WindowModal
    title: "Customized content"
    onRejected: lastChosen.text = "Rejected"
    onAccepted: lastChosen.text = "Accepted " +
                (clickedButton === StandardButton.Retry ? "(Retry)" : "(OK)")
    onButtonClicked: if (clickedButton === StandardButton.Retry) lastChosen.text = "Retry"
    contentItem: Rectangle {
        color: "lightskyblue"
        implicitWidth: 400
        implicitHeight: 500
        Label {
            text: "Hello blue sky!"
            color: "navy"
            anchors.centerIn: parent
        }
    }
}

