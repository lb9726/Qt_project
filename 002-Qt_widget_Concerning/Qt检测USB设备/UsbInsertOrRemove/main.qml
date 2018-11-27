import QtQuick 2.7
import QtQuick.Window 2.2

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Usb Detected")
    property string usbaction: "hello"
    Item {
        id: name
        anchors.fill: parent
        Text {
            anchors.centerIn: parent
            text: usbaction
        }
    }

    Connections {
        target: Usbdetect
        onUActionChanged: {
            console.log("recieve signals");
            usbChangeStatus(uact);
            console.log("uact = ", uact);
        }
    }
    function usbChangeStatus(act)
    {
        usbaction = act;
        console.log("usbaction = ", usbaction);
    }
}
