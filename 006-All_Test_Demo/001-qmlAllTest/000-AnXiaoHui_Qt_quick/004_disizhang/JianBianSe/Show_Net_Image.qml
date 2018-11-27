import QtQuick 2.0
import QtQuick.Controls 1.2

Rectangle {
    width: 480
    height: 320
    color: "#121212"

    BusyIndicator {
        id: busy
        running: true
        anchors.centerIn: parent
        z: 2
    }

    Text {
        id: stateLabel
        visible: false
        anchors.centerIn: parent
        z: 3
    }

    Image {
        id: imageViewer
        asynchronous: true;
        cache: false
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
        onStatusChanged: {
            if (imageViewer.status === Image.Loading)
            {
                busy.running = true
                stateLabel.visible = false
                console.log("is Loading");
            }
            else if (imageViewer.status === Image.Ready)
            {
                busy.running = false
                console.log("is Ready");
            }
            else if (imageViewer.status === Image.Error)
            {
                busy.running = false
                stateLabel.visible = true
                stateLabel.text = "ERROR"
                console.log("is Error");
            }
            else
            {
                console.log("is other status")
            }
        }
    }

    Component.onCompleted: {
        console.log("imageViewer.status = ", imageViewer.status)
        imageViewer.source = "https://goss.veer.com/creative/vcg/veer/800water/veer-100617352.jpg"
        console.log("imageViewer.status = ", imageViewer.status)
    }
}

/*
Image对象，设置了asynchronous属性为true，不过对于网络资源Image默认异步加载，
这个属性不起作用，只有你想异步加载本地资源时才需要设置它。cache属性设置false，
告诉Image不用缓存图片。fillMode属性设置了等比缩放模式。

onStatusChanged是信号处理器，Image的status属性变化时会发射statusChanged()信号。
在信号处理器代码块中，我们通过Image对象的id访问它的status属性，根据不同的状态来更新界面。
*/
