import QtQuick 2.0

Item {
    property string wifipicturepath: "images/lift_network.png"
    property int i: 0
    width: wifi.width; height: wifi.height
    Image {
        id: wifi
        source: wifipicturepath
        anchors.centerIn: parent
    }
    Timer {
        interval: 1500; running: true; repeat: true;
        onTriggered: {
            i++;
            if (0 == i%2)
            {
                wifipicturepath = "images/zn_lift_network.png"
                wifi.source = wifipicturepath
            }
            else if (1 == i %2)
            {
                wifipicturepath = 'images/lift_network.png'
                wifi.source = wifipicturepath
            }
            console.log("wifipicturepath = ",wifipicturepath, "i=", i)
            if (i > 3)
                i = 0;
        }
    }
}

