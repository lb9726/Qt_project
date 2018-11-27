import QtQuick 2.5
import "editor.js" as Code

Column {
    spacing: 10
    Rectangle {
        width: parent.width
        height: parent.height/8
        color: "transparent"
        Text {
            text: "Tools"
            color: "white"
            anchors.centerIn: parent
            font.pixelSize: parent.height/3
            font.family: "微软雅黑"
        }
    }

    GridView {
        id: id_toollist
        width: parent.width
        height: parent.height*7/8
        cellWidth: id_toollist.width/2; cellHeight: id_toollist.height/8
        delegate: DragItem {
            dragQmlFile: "DragIcon.qml"
            image: "qrc: /images/icon/"+(index+1)+".png"
            dragImage: "qrc: /images/icon/"+(index+1)+".png"
            parentItem: dropArea
            width: id_toollist.width/2
            height: id_toollist.height/8
            panelName: panel
            toolName: name
            allowIndex: allowNum
        }
        interactive: false
        model: ctrlModel;
        focus: true
    }

    ListModel {
        id: ctrlModel
        ListElement {name: "Arrow"; panel: "Arrow"; allowNum: 1}
        ListElement {name: "Floor"; panel: "Floor"; allowNum: 1}
        ListElement {name: "Function"; panel: "Function"; allowNum: 1}
        ListElement {name: "Text"; panel: "Text"; allowNum: 1}
        ListElement {name: "Title"; panel: "Title"; allowNum: 1}
        ListElement {name: "Date"; panel: "Date"; allowNum: 1}
        ListElement {name: "Time"; panel: "Time"; allowNum: 1}
        ListElement {name: "Media"; panel: "Media"; allowNum: 1}
        ListElement {name: "Picture"; panel: "Picture"; allowNum: 1}
        ListElement {name: "Video"; panel: "Video"; allowNum: 1}
    }

    state: gAutoMode ? "Auto" : "Normal"

    states: [
        State {
            name: "Normal"
            PropertyChanges { target: id_toolbox; enabled: true;opacity: 1 }
        },
        State {
            name: "Auto"
            PropertyChanges { target: id_toolbox; enabled: false;opacity: 0.6 }
        }
    ]

    function createdPanel(name) {
        var num = 0;
        var i = 0
        for( i = 0; i<ctrlModel.count; i++) {
            if(ctrlModel.get(i).panel === name) {

                //num = parseInt(ctrlModel.get(i).allowNum);
                //console.log(name,num)
                //if(num >0 )
                    //ctrlModel.setProperty(i,"allowNum",(num-1));
                //else {
                  //  ctrlModel.setProperty(i,"allowNum",0);
                //}
                //console.log("createdPanel: ",name,num);
                ctrlModel.setProperty(i,"allowNum",0);
                break;
            }
        }
    }

    function destroyPanel(name) {
        var num = 0;
        var i = 0;
        for( i = 0; i < ctrlModel.count; ++i) {
            if(ctrlModel.get(i).panel === name)
            {
                //num = parseInt(ctrlModel.get(i).allowNum);
                ctrlModel.setProperty(i, "allowNum", 1);
                //console.log("destroyPanel: ",name,num);
                break;
            }
        }
    }
}
