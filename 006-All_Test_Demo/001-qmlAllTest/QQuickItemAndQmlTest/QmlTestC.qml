import QtQuick 2.0
import lb2616.tools.QmlScreenC 1.0
import QtQuick.Controls 2.0

QmlScreenC {
    id: qmlScreenC
    objectName: "InstanceQmlScreenC"
    width: 600
    height: 600
    Rectangle {
        id: redRect
        objectName: "QmlScreenCRedRect"
        width: 300
        height: 300
        color: "red"
        border.color: "green"
        border.width: 3
        Text {
            anchors.centerIn: parent
            text: qsTr("is InstanceQmlScreenC")
        }
    }
    ComboBox {
        id: combox
        objectName: "Acombox"
        anchors.top: redRect.bottom
        currentIndex: 2
        model: ["qmlScreenA", "qmlScreenB", "qmlScreenC"]
        onCurrentIndexChanged: {
            console.log("currentIndex is chanage in qmlScreenC", currentIndex);
            FrameWork.changeUi(currentIndex);
        }
        function changeColor(currentIndex)
        {
            if (1 === currentIndex)
            {
                redRect.color = "orange"
            }
            else if (2 === currentIndex)
            {
                redRect.color = "yellow"
            }
            else
            {
                redRect.color = "red"
            }
            console.log("currentIndex is chanage in qmlScreenC", currentIndex);
        }
    }
}
