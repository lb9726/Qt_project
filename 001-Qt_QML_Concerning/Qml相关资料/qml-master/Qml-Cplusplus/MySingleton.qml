import QtQuick 2.0
import QtQuick.Window 2.0
import lb2616.tools.MySingleton 1.0
Window {
    visible: true
    width: 360; height: 360
    title: "Using MySingleton"
    color: "lightblue"

    Item {
        id: item
        width: parent.width;
        height: parent.height
        property int qmlAge: MySingleton.age

        MouseArea {
            anchors.fill: parent;
            onPressed: console.log(item.qmlAge)
            onReleased: {
                item.qmlAge = MySingleton.changAge()
                console.log(item.qmlAge)
            }
        }
    }
}

