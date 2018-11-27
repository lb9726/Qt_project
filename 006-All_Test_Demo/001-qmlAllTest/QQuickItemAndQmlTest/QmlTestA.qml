import QtQuick 2.0
import lb2616.tools.QmlScreenA 1.0
import QtQuick.Controls 2.0

QmlScreenA {
    id: qmlScreenA
    objectName: "InstanceQmlScreenA"
    width: 500
    height: 500
    rotation: 0
//    color: "green"
    Rectangle {
        id: redRect
        objectName: "QmlScreenARedRect"
        width: 300
        height: 300
        color: "red"
        border.color: "green"
        border.width: 3
        anchors.centerIn: parent
        Text {
            anchors.centerIn: parent
            text: qsTr("is InstanceQmlScreenA")
        }
    }
    ComboBox {
        id: combox
        objectName: "Acombox"
        anchors.top: redRect.bottom
        currentIndex: 0
        model: ["qmlScreenA", "qmlScreenB", "qmlScreenC"]
        onCurrentIndexChanged: {
            console.log("currentIndex is chanage in qmlScreenA", currentIndex);
            FrameWork.changeUi(currentIndex);
//            changeColor(currentIndex);
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
            console.log("currentIndex is chanage in qmlScreenA", currentIndex);
        }
    }
}
