import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.2
import QtQuick.Dialogs 1.2

Item{
    id:root
    property string m_OpenProjectPath: ""
    property string m_NewPath: ""
    property alias m_NewName: id_proNameInput.text

    property bool openStatus:false

    Button {
        width:windowWidth/20 > 50 ? 50: windowWidth/20
        height: windowWidth/20 > 50 ? 50: windowWidth/20
        opacity: pressed ? themeOpacity : 1
        anchors{
            left:parent.left
            top:parent.top
            margins: windowWidth > windowHeight  ? windowHeight/20 : windowWidth/20
        }

        style: ButtonStyle {
            background: Image {
                source: "qrc:/images/back.png"
            }

        }

        onClicked: {
            uiLogicalEngine(3,1)
        }
    }

    Item{
        width: parent.width*3/4
        height: parent.height*3/4
        anchors.centerIn: parent
        Row{
            anchors.fill: parent
            spacing: 5

            Column{
                id:id_newProjectArea
                width: parent.width*6/10
                height: parent.height
                spacing: height/14
                opacity: enabled ? 1: 0.5
                Item{
                    width: parent.width
                    height: parent.height/10
                    Text{
                        anchors.fill: parent
                        text:"New Project"
                        font.family:"微软雅黑"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: height/2
                        color: "white"
                    }
                }

                Row{
                    width: parent.width
                    height: parent.height/10
                    Text{
                        width: parent.width/3
                        height: parent.height
                        text:"Device Model:"
                        font.family:"微软雅黑"
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment:  Text.AlignVCenter
                        //wrapMode: Text.WrapAnywhere
                        font.pixelSize: height/3
                        clip: true
                    }
                    ComboBox {
                        id:id_templeteCombo
                        //currentIndex: 2
                        //model: id_templetemodel
                        width: parent.width*2/3
                        height: parent.height
                    }
                }

                Row{
                    width: parent.width
                    height: parent.height/10
                    Text{
                        width: parent.width/3
                        height: parent.height
                        font.family:"微软雅黑"
                        text:"Project Name:"
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment:  Text.AlignVCenter
                        //wrapMode: Text.WrapAnywhere
                        font.pixelSize: height/3
                    }
                    Rectangle{
                        width: parent.width*2/3
                        height: parent.height
                        color: "white"
                        TextInput {
                            id:id_proNameInput
                            anchors.fill: parent
                            font.family:"微软雅黑"
                            verticalAlignment: Text.AlignVCenter
                            font.pixelSize: height/3
                            color: "black"
                            maximumLength: 8
                        }
                    }
                }

                Row{
                    width: parent.width
                    height: parent.height/10
                    //spacing: width/3
                    Text{
                        width: parent.width/3
                        height: parent.height
                        font.family:"微软雅黑"
                        text:"Project Path:"
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment:  Text.AlignVCenter
                        //wrapMode: Text.WrapAnywhere
                        font.pixelSize: height/3
                    }

                    Rectangle {
                        color: viceColor
                        width: parent.width/3
                        height: parent.height
                        //anchors.horizontalCenter: parent.horizontalCenter
                        Text{
                            text:"select"
                            font.family:"微软雅黑"
                            anchors.centerIn: parent
                            color: "white"
                            //font.pixelSize: parent.height/3
                            //wrapMode: Text.WrapAnywhere
                            font.pixelSize: parent.height/3
                        }
                        MouseArea{
                            anchors.fill: parent

                            onClicked:{
                                id_forldSelect.open();
                            }

                            onPressed:parent.opacity = themeOpacity;
                            onReleased: parent.opacity = 1;
                        }
                    }
                }

                Row{
                    //opacity: 0
                    width: parent.width
                    height: parent.height/10
                    //spacing: width/3
                    Text{
                        width: parent.width/3
                        height: parent.height
                        text:"        Path:"
                        font.family:"微软雅黑"
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment:  Text.AlignVCenter
                        //wrapMode: Text.WrapAnywhere
                        font.pixelSize: height/3
                    }
                    Text{
                        id:id_projectPath
                        width: parent.width*2/3
                        height: parent.height
                        text:m_NewPath
                        font.family:"微软雅黑"
                        verticalAlignment: Text.AlignVCenter
                        color: "white"
                        //wrapMode: Text.WrapAnywhere
                        font.pixelSize: height/3
                    }
                }

                Item{
                    width: parent.width
                    height: parent.height/10
                    Rectangle {
                        color: "grey"
                        width: parent.width/2
                        height: parent.height
                        anchors.horizontalCenter: parent.horizontalCenter
                        Text{
                            text:"Create"
                            font.family:"微软雅黑"
                            anchors.centerIn: parent
                            color: "white"

                            font.pixelSize: parent.height/2
                        }
                        MouseArea{
                            anchors.fill: parent
                            onClicked:{
                                createProject();
                            }

                            onPressed:parent.opacity = themeOpacity;
                            onReleased: parent.opacity = 1;
                        }
                    }
                }
            }

            Item{
                width: parent.width/10
                height: parent.height
                Rectangle{
                    height: parent.height
                    width: 3
                    anchors.centerIn: parent
                    opacity: 0.8
                    radius: 3
                }
            }

            Column{
                width: parent.width*3/10
                height: parent.height
                spacing: height * 0.185

                Item{
                    width: parent.width
                    height: parent.height/10
                    Text{
                        anchors.fill: parent
                        text:"Open Project"
                        font.family:"微软雅黑"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: height/2
                        color: "white"
                    }
                }

                Item{
                    width: parent.width
                    height: parent.height/10
                    Rectangle {
                        color: viceColor
                        width: parent.width
                        height: parent.height
                        anchors.horizontalCenter: parent.horizontalCenter
                        Text{
                            font.family:"微软雅黑"
                            text:"Click to open project"
                            anchors.centerIn: parent
                            color: "white"
                            //wrapMode: Text.WrapAnywhere
                            font.pixelSize: parent.height/3
                        }
                        MouseArea{
                            anchors.fill: parent
                            onClicked:{
                                id_openProject.open();
                            }

                            onPressed:parent.opacity = themeOpacity;
                            onReleased: parent.opacity = 1;
                        }
                    }
                }

                Item{
                    width: parent.width
                    height: parent.height/10
                    clip: true
                    Text{
                        font.family:"微软雅黑"
                        text:m_OpenProjectPath
                        anchors.centerIn: parent
                        color: "white"
                        font.pixelSize: parent.height/3
                        horizontalAlignment: Text.AlignRight
                        //wrapMode: Text.WrapAnywhere
                    }
                }

                Item{
                    width: parent.width
                    height: parent.height/10
                    enabled: m_OpenProjectPath == "" ? false:true
                    opacity: enabled ? 1 : themeOpacity
                    Rectangle {
                        color: "grey"
                        width: parent.width*3/4
                        height: parent.height
                        anchors.horizontalCenter: parent.horizontalCenter
                        Text{
                            font.family:"微软雅黑"
                            text:"Load"
                            anchors.centerIn: parent
                            color: "white"
                            font.pixelSize: parent.height/2
                        }
                        MouseArea{
                            anchors.fill: parent
                            onClicked:{
                                loaderProject();
                            }

                            onPressed:parent.opacity = themeOpacity;
                            onReleased: parent.opacity = 1;
                        }
                    }
                }
            }
        }
    }

    FileDialog {
        id: id_openProject
        title: "Please choose project file"
        //folder: shortcuts.home
        selectExisting : true
        selectFolder : false
        selectMultiple : false
        nameFilters: [ "project files (*.ui)"]
        onAccepted: {
            var temp = id_openProject.fileUrl.toString()
            m_OpenProjectPath  = temp.substring(8,temp.length);
        }
    }

    FileDialog {
        id: id_forldSelect
        title: "Please folder of project"
        //folder: shortcuts.home
        selectExisting : true
        selectFolder : true
        selectMultiple : false
        onAccepted: {
            var temp = id_forldSelect.folder.toString()
            m_NewPath  = temp.substring(8,temp.length)
        }
    }

    Component.onCompleted: {
        var list = Templete.getTemplete();
        if(list.length >0){
            id_templeteCombo.model = list;
            id_templeteCombo.currentIndex = 0;
        }else{
            id_templeteCombo.model = null;
            id_newProjectArea.enabled = false
        }
    }

    function createProject(){
        showBusy();
        if(m_NewName == "" || m_NewPath == ""){
            hideBusy();
            messageDialog.text = "project name or project path can't empty."
            messageDialog.open()
        }else if(id_templeteCombo.currentIndex <0 ||id_templeteCombo.currentIndex >id_templeteCombo.count) {
            hideBusy();
            messageDialog.text = "project name or project path can't empty."
            messageDialog.open()
        }else{
            var flag = Templete.createTemplete(m_NewPath,m_NewName,id_templeteCombo.currentIndex);
            if(flag){
                rootWindow.projectStatus = true;
                rootWindow.openProject();
            }else{
                rootWindow.projectStatus = false;
            }
        }
        hideBusy();
    }

    function loaderProject(){
        showBusy();
        if(m_OpenProjectPath == ""){
            messageDialog.text = "project file can't empty."
            messageDialog.open()
        }else{
            var flag = GUiEditor.loadUiFile(m_OpenProjectPath);
            if(flag){
                rootWindow.projectStatus = true;
                rootWindow.openProject();
            }else{
                rootWindow.projectStatus = false;
            }
        }
        hideBusy();
    }

    MessageDialog {
        id: messageDialog
        title: "Infomation"
    }
}
