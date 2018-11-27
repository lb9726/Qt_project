import QtQuick 2.3
import QtQuick.Window 2.2

Window {
    width: 360;
    height: 360;
    visible: true

        Rectangle {
            width: 80;
            height: 80;
            color: "red";
            focus: true;
            id: transformRect;
            anchors.centerIn: parent;

        PinchArea {
            enabled: true
            anchors.fill: parent
            pinch.maximumScale: 50;
            pinch.minimumScale: 0.2;
            pinch.minimumRotation: 0;
            pinch.maximumRotation: 90;
            pinch.target: transformRect

            onPinchStarted: {
                pinch.accepted = true;
            }
            onPinchUpdated: {
                transformRect.scale *= pinch.scale;
                transformRect.rotation += pinch.rotation;
            }
            onPinchFinished: {
                transformRect.scale *= pinch.scale;
                transformRect.rotation += pinch.rotation;
            }
        }
        }
}
