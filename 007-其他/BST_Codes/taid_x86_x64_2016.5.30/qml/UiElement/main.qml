
import QtQuick 2.3
import QtQuick.Window 2.2

Window {
    id: root;
    visible: true;
    width: 800;
    height: 600;


   Row{
        anchors.centerIn: parent
        Rectangle{
            z:-1
            width: 200
            height: 600
            color: "transparent"
            Rectangle {
                id: dragItem;
                width: 60;
                height: 60;
                clip:true
                anchors.centerIn: parent
                Drag.active: dragArea.drag.active;
                Drag.supportedActions: Qt.CopyAction;
                Drag.dragType: Drag.Automatic;
                Drag.mimeData: {"colorinfo": color,"note": "note"};
                color: "red"

                MouseArea {
                    id: dragArea;
                    anchors.fill: parent;
                    drag.target: parent;
//                    onPressed: {
//                        //dragItem.Drag.start(Qt.CopyAction)
//                        //console.log("dragArea Pressed")
//                    }
//                    onPressAndHold: {
//                        Drag.active = true
//                        Drag.startDrag(Qt.CopyAction)
//                    }
                }

                Drag.onDragStarted: {
                    console.log("onDragStarted")
                }

                Drag.onDragFinished: {
                    Drag.cancel();
                    console.log("onDragFinished")
                }
                Component.onCompleted: {
                    //Drag.active = true
                    //Drag.startDrag(Qt.CopyAction)
                }
            }
        }

        DropArea {
            id: dropContainer;
            z: 10;
            width: 400
            height: 600

            onEntered: {
                //drag.accepted = false;
                followArea.color = drag.getDataAsString("color");
            }

            onPositionChanged: {
                drag.accepted = true;
                followArea.x = drag.x - 4;
                followArea.y = drag.y - 4;
            }

            onDropped: {
                //console.log(drop.getDataAsString("note"));
                var component = Qt.createComponent("MyButton.qml");
                if (component.status === Component.Ready)
                    component.createObject(destArea, {"width": 100, "height": 100});


                /*
                var obj = dragColor.createObject(destArea,{
                                                     "x": drop.x,
                                                     "y": drop.y,
                                                     "width": 80,
                                                     "height": 80,
                                                     "focus": true
                                                 });
                */
                drop.accept()

            }

            Rectangle {
                id: followArea;
                z: 2;
                width: 28;
                height: 28;
                border.width: 2;
                border.color: "yellow";
                visible: parent.containsDrag;
            }

            Rectangle {
                id: destArea;
                anchors.fill: parent;
                color: "lightsteelblue";
                border.width: 2;
                border.color: parent.containsDrag ? "blue" : "gray";
            }
        }
    }

    Component {
        id: dragColor
        Rectangle {
            color: focus? "green":"red"
        }
    }
}
