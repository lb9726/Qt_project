import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.2
import QtQml.Models 2.2
import QtQuick.Dialogs 1.2
Item{
    //property string selectUiFile: ""
    property string localUiFile: ""
    property string modelUiFile: ""
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
            uiLogicalEngine(5,1);
        }
    }

    ExclusiveGroup { id: template_RadioGroup }

    Rectangle{
        id:device_select_area
        width: parent.width*3/4
        height: parent.height/2
        anchors.centerIn: parent
        anchors.verticalCenterOffset: -parent.height/8
        color: "transparent"

        Row{
            anchors.fill: parent
            spacing: 20
            Column{
                width: parent.width/2
                height: parent.height
                Item {
                    width: parent.width
                    height: parent.height/2
                    //color: "red"
                    RadioButton{
                        id:id_selectLocal
                        anchors.centerIn: parent
                        text:" 从本地获取"
                        width:parent.width/2
                        height:parent.height/4
                        exclusiveGroup:template_RadioGroup
                        checked:true
                        style: RadioButtonStyle {
                            indicator: Rectangle {
                                implicitWidth: control.height
                                implicitHeight: control.height
                                radius: control.height/2
                                border.color: "white"
                                border.width: 1
                                color:"transparent"
                                Rectangle {
                                    anchors.fill: parent
                                    visible: control.checked
                                    color: "white"
                                    radius: control.height/2
                                    anchors.margins: control.height/5
                                }
                            }
                            label:Text{
                                color: "white";
                                text:control.text;
                                font.bold: true;
                                font.pixelSize: control.height/2
                            }
                            spacing: control.width/8
                        }

                        onClicked: {
                            id_submit.enabled = false
                            if(localUiFile != "")
                                id_submit.enabled = true
                        }
                    }
                }
                Item{
                    id:id_localInput
                    width: parent.width
                    height: parent.height/2
                    enabled: id_selectLocal.checked ? true: false
                    opacity: enabled ? 1 :0.6
                    Row{
                        width: parent.width
                        height: parent.height/2
                        anchors.centerIn: parent
                        spacing: 10
                        Rectangle {
                            width: parent.width *6/8
                            height: parent.height
                            color: viceColor
                            Text{
                                id:local_uifile
                                text:"";
                                anchors.verticalCenter:   parent.verticalCenter
                                color: "white";
                                font.pixelSize: (parent.height*3/4) /(text.length/9)
                                clip:true
                                wrapMode:Text.Wrap
                            }
                        }

                        Rectangle {
                            width: parent.width/8
                            height: parent.height
                            color: viceColor
                            Text{
                                text:"...";
                                anchors.centerIn: parent
                                color: "white";
                                font.pixelSize: parent.height*3/8
                            }
                            MouseArea{
                                anchors.fill: parent
                                onClicked:{
                                    fileDialog.open();
                                }
                                onPressed:parent.opacity = themeOpacity;
                                onReleased: parent.opacity = 1;
                            }
                        }

                    }


                }
            }

            Column{
                width: parent.width/2
                height: parent.height
                Item {
                    width: parent.width*3/4
                    height: parent.height/2
                    //color: "red"
                    RadioButton{
                        id:id_selectModel
                        anchors.centerIn: parent
                        text:"从模板新建";
                        width:parent.width/2
                        height:parent.height/4
                        exclusiveGroup:template_RadioGroup
                        style: RadioButtonStyle {
                            indicator: Rectangle {
                                implicitWidth: control.height
                                implicitHeight: control.height
                                radius: control.height/2
                                border.color: "white"
                                border.width: 1
                                color:"transparent";
                                Rectangle {
                                    anchors.fill: parent
                                    visible: control.checked
                                    color: "white";
                                    radius: control.height/2
                                    anchors.margins: control.height/5
                                }
                            }
                            label:Text{
                                color: "white";
                                text:control.text
                                font.bold: true
                                font.pixelSize: control.height/2
                            }
                            spacing: control.width/8
                        }
                        onClicked: {
                            id_submit.enabled = false
                            if(modelUiFile!= "")
                                id_submit.enabled = true
                        }
                    }
                }

                Item {
                    id:id_modelInput
                    width: parent.width
                    height: parent.height/2
                    enabled: id_selectModel.checked ? true: false
                    opacity: enabled ? 1 :0.6
                    Row{
                        height: parent.height/2
                        width: parent.width
                        anchors.centerIn: parent
                        Rectangle{
                            height: parent.height
                            width: parent.width*4/5
                            color: "white";
                            Text{
                                id:template_name
                                anchors.fill: parent
                                text:"选择设备型号"
                                verticalAlignment: Text.AlignVCenter
                                font.pixelSize: height/2
                            }

                            MouseArea{
                                anchors.fill: parent
                                onClicked: {
                                    template_selectLoader.sourceComponent = template_list
                                }
                            }
                        }
                        Rectangle{
                            height: parent.height
                            width: parent.width/5
                            color: "white"
                            Image{
                                height: parent.height/2
                                width: parent.width/2
                                source: "qrc:/images/down_Arror.png"
                                anchors.centerIn: parent
                                antialiasing:true
                            }
                        }
                    }
                }
            }
        }
    }

    Rectangle {
        id:id_submit
        color: viceColor
        anchors{
            top:device_select_area.bottom
            topMargin: parent.height/8
            horizontalCenter: device_select_area.horizontalCenter
        }
        width: device_select_area.width/3
        height: parent.height/8
        property string localUiFile: ""
        property string modelUiFile: ""
        enabled:false
        opacity: enabled ? 1:0.6
        Text{
            text:"确定"
            anchors.centerIn: parent
            color: "white"
            font.pixelSize: parent.height/2
        }
        MouseArea{
            anchors.fill: parent
            onClicked:{
                uiLogicalEngine(5,0);
                if(id_selectLocal.checked){
                    GUiEditor.selectedUiFile(localUiFile)
                }else{
                    GUiEditor.selectedUiFile(modelUiFile)
                }
            }
            onPressed:parent.opacity = themeOpacity;
            onReleased: parent.opacity = 1;
        }
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        //folder: shortcuts.home
        nameFilters: [ "UI files (*.ui)", "All files (*)" ]
        onAccepted: {
            var path = fileDialog.fileUrl.toString();
            var pre = path.lastIndexOf("file:///");
            if(pre >= 0){
                local_uifile.text = localUiFile = path.substring(8);
                id_submit.enabled = true
            }
            //local_uifile.text = filepath.toString().substring(7)
        }
    }

    Loader{
        id:template_selectLoader
        anchors.fill: parent
        z:10
    }

    Component{
        id:template_list
        Item{
            anchors.fill: parent
            MouseArea{
                anchors.fill: parent
                z:-1
            }
            Item{
                id:listview_container
                width: parent.width/2
                height: parent.height*3/4
                anchors.centerIn: parent
                clip:true
                Rectangle{
                    color: viceColor
                    anchors.fill: parent
                    z:-1
                    //opacity: themeOpacity
                }

                ListView {
                    anchors.fill: parent
                    model: template_listModel
                    delegate: Item {
                        width: listview_container.width
                        height:listview_container.height/10

                        Rectangle{
                            width: parent.width
                            height: 2
                            opacity: 0.4
                            z:-1
                            anchors.bottom: parent.bottom
                        }

                        Text{
                            text: name
                            color: "white"
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            //anchors.horizontalCenterOffset: listview_container.height/4
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                        }

                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                //uifileSelect(file)
                                id_submit.enabled = true
                                modelUiFile = file
                                template_name.text = name
                                template_selectLoader.sourceComponent = null
                            }
                        }

                    }
                }
            }

        }
    }

    ListModel {
        id: template_listModel
        ListElement {
            name: "Apple"
            file: "2.45"
        }
        ListElement {
            name: "Orange"
            file: "3.25"
        }
        ListElement {
            name: "Banana"
            file: "1.95"
        }
    }
}

