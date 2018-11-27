import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtMultimedia 5.4

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Item {
        anchors.fill: parent
        /*
            This example was working fine in Qt 5.4.1
            It isn't working in 5.4.2 and 5.5.0
            Hopefully, someone will fix video streaming in 5.5.1
        */
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: spacing

            ComboBox {
                Layout.fillWidth: true
                model: WebCamsModel {
                    id: comboBoxModel
                }
                onCurrentIndexChanged: video.source = comboBoxModel.get(currentIndex).stream
            }


            Video {
                id: video
                Layout.fillWidth: true
                Layout.fillHeight: true
                autoPlay: true
                source: comboBoxModel.get(0).stream
            }
        }
    }
}
