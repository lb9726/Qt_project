import QtQuick 2.5
import QtQuick.Window 2.2

import QtQuick 2.3
import QtQuick.Window 2.2

Window {
    id: root;
    visible: true;
    width: 480;
    height: 400;

    //drag source item should not use anchors to layout! or drag will failed
    Component {
        id: dragColor;
        Rectangle {
            id: dragItem;
            x: 0;
            y: 0;
            width: 60;
            height: 60;
            Drag.active: dragArea.drag.active;
            Drag.supportedActions: Qt.CopyAction;
            Drag.dragType: Drag.Automatic;
            Drag.mimeData: {"color": color, "width": width, "height": height};

            MouseArea {
                id: dragArea;
                anchors.fill: parent;
                drag.target: parent;

                onReleased: {
                    if(parent.Drag.supportedActions == Qt.CopyAction){
                        dragItem.x = 0;
                        dragItem.y = 0;
                    }
                }
            }
        }
    }

    Row {
        id: dragSource;
        anchors.top: parent.top;
        anchors.left: parent.left;
        anchors.margins: 4;
        anchors.right: parent.right;
        height: 64;
        spacing: 4;
        z:-1;
        Loader {
            width: 60;
            height: 60;
            z: 2;
            sourceComponent: dragColor;
            onLoaded: item.color = "red";
        }
        Loader {
            width: 60;
            height: 60;
            z: 2;
            sourceComponent: dragColor;
            onLoaded: item.color = "black";
        }
        Loader {
            width: 60;
            height: 60;
            z: 2;
            sourceComponent: dragColor;
            onLoaded: item.color = "blue";
        }
        Loader {
            width: 60;
            height: 60;
            z: 2;
            sourceComponent: dragColor;
            onLoaded: item.color = "green";
        }
    }

    DropArea {
        id: dropContainer;
        anchors.top: dragSource.bottom;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.bottom: parent.bottom;
        z: -1;

        onEntered: {
            drag.accepted = true;
            followArea.color = drag.getDataAsString("color");
            console.log("onEntered, formats - ", drag.formats, " action - ", drag.action);
        }

        onPositionChanged: {
            drag.accepted = true;
            followArea.x = drag.x - 4;
            followArea.y = drag.y - 4;
        }

        onDropped: {
            console.log("onDropped - ", drop.proposedAction);
            console.log("data - ", drop.getDataAsString("color"));
            console.log("event.x - ", drop.x, " y- ", drop.y);
            console.log("event class = ", drop);
            if(drop.supportedActions == Qt.CopyAction){
                var obj = dragColor.createObject(destArea,{
                                                     "x": drop.x,
                                                     "y": drop.y,
                                                     "width": parseInt(drop.getDataAsString("width")),
                                                     "height": parseInt(drop.getDataAsString("height")),
                                                     "color": drop.getDataAsString("color"),
                                                     "Drag.supportedActions": Qt.MoveAction,
                                                     "Drag.dragType": Drag.Internal

                                                 });
            }else if(drop.supportedActions == Qt.MoveAction){
                console.log("move action, drop.source - ", drop.source, " drop.source.source - ", drop.source.source);
            }
            drop.acceptProposedAction();
            drop.accepted = true;
        }

        Rectangle {
            id: followArea;
            z: 2;

            width: 68;
            height: 68;
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

