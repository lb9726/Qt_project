import QtQuick 2.0
import lb2616.tools.Protocoldeal 1.0
Item {
    Column {
        Protocoldeal{
            id: proc
        }
        Text {
            id: time
//            text: Qt.formatDateTime(new Date(), "hh:mm")
            color: "white"
            font.family: "KONE Information_v12"
            font.pixelSize: 56
            anchors.left: date.left
        }
        Text {
            id: date
//            text: Qt.formatDateTime(new Date(), "dd.M.yyyy")
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
            target: proc
            onSendTimerChanged: {
                console.log("tim = ", tim);
                time.text = tim;
//                date.text = dat;
            }
        }
        Connections{
            target: proc
            onSendDateChanged: {
                console.log("dat = ", dat);
                date.text = dat;
            }
        }
    }
}

