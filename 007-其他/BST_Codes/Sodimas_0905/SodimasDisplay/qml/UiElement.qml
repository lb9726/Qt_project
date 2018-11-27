import QtQuick 1.1

Item {
    id:id_element
//    Rectangle{
//        border.width: 2
//        border.color: "blue"
//        anchors.fill: parent
//        color: "transparent"
//        Text {
//            anchors.centerIn: parent
//            text: qsTr(id_element.objectName)
//        }
//    }

    function setGeometry(rect){
        ///console.log(objectName ,": ",rect.x,",",rect.y,",",rect.width,",",rect.height);
        x = rect.x
        y = rect.y
        width = rect.width
        height = rect.height
    }
}
