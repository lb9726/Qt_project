import QtQuick 2.0
Item {
    Column {
        Text {
            id: time
//            text: " "/*Qt.formatDateTime(new Date(), "hh:mm")*/
            text: Qt.formatDateTime(new Date(), "hh:mm")
            color: "white"
            font.family: "KONE Information_v12"
            font.pixelSize: 56
            anchors.left: date.left
        }
        Text {
            id: date
//            text: " "// Qt.formatDateTime(new Date(), "dd.M.yyyy")
            text: Qt.formatDateTime(new Date(), "dd.MM.yyyy")
            color: "white"
            font.family: "KONE Information_v12"
            font.pixelSize: 30
        }
        //        Timer {
        //            interval: 1000; running: true; repeat: true
        //            onTriggered: time.text = Qt.formatDateTime(new Date(), "hh:mm")
        //        }
        //        Timer {
        //            interval: 1000; running: true; repeat: true
        //            onTriggered: date.text = Qt.formatDateTime(new Date(), "dd.M.yyyy")
        //        }

        Connections{
            target: Mypro
            onSendTimerChanged: {
                console.log("tim = ", tim);
                time.text = tim;
//                date.text = dat;
            }
        }
        Connections{
            target: Mypro
            onSendDateChanged: {
                console.log("dat = ", dat);
                date.text = dat;
            }
        }
    }
}

