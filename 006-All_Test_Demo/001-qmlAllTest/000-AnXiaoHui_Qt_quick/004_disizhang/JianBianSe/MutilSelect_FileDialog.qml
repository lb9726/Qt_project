import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import QtQuick.Dialogs 1.1

Item {
    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        nameFilters: ["Image Files (*.jpg *.png *.gif)", "BitMap File (\
            *.bmp)", "* (*.*)"]
        selectedNameFilter: "Image Files (*.jpg *.png *.gif)"
        selectMultiple: true
        onAccepted: {
            console.log(fileDialog.fileUrls)
        }
    }
    Button {
        width: 100
        height: 30
        text: "clicked me"
        onClicked: {
            fileDialog.open()
        }
    }
}
