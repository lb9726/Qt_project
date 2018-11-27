import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.2
import QtQuick.Dialogs 1.2

Item{
    property string localUiFile: ""
    property bool loadStatu: false;

    property int padSize: windowWidth > windowHeight ? windowHeight/4:windowWidth/4

    Column{
        anchors.centerIn: parent
        spacing: padSize
        Rectangle{
            visible:  !projectStatus
            //opacity: enabled ? 1:themeOpacity
            anchors.horizontalCenter: parent.horizontalCenter
            color: viceColor
            width: padSize
            height: padSize
            radius: height
            clip:true
            Text{
                text:"Project"
                font.pixelSize: windowWidth/40
                anchors.centerIn: parent
                font.family:"微软雅黑"
                color: "white"
            }
            MouseArea{
                anchors.fill: parent
                onPressed: {
                    parent.opacity = themeOpacity
                }
                onReleased:  {
                    parent.opacity = 1
                }
                onClicked: {
                    uiLogicalEngine(2,0)
                }
            }
        }
        Row{
            enabled: projectStatus
            opacity: enabled ? 1 : themeOpacity
            spacing: padSize*1.5
            Rectangle{
                color: viceColor
                width: padSize
                height: padSize
                radius: padSize/20
                clip:true
                Text{
                    text:"Editor"
                    font.family:"微软雅黑"
                    font.pixelSize: windowWidth/40
                    anchors.centerIn: parent
                    color: "white"
                }
                MouseArea{
                    anchors.fill: parent
                    onPressed: {
                        parent.opacity = themeOpacity
                    }
                    onReleased:  {
                        parent.opacity = 1
                    }
                    onClicked: {
                        uiLogicalEngine(6,3)
                    }
                }
            }
            Rectangle{
                color: viceColor
                width: padSize
                height: padSize
                radius: padSize/20
                clip:true
                Text{
                    text:"Publish"
                    font.family:"微软雅黑"
                    font.pixelSize: windowWidth/40
                    anchors.centerIn: parent
                    color: "white"
                }
                MouseArea{
                    anchors.fill: parent
                    onPressed: {
                        parent.opacity = themeOpacity
                    }
                    onReleased:  {
                        parent.opacity = 1
                    }
                    onClicked: {
                        uiLogicalEngine(6,5)
                    }
                }
            }
        }
    }

    FileDialog {
        id: fileDialog
        title: "select ui file"
        ////folder: shortcuts.home
        nameFilters: [ "UI files (*.ui)", "All files (*)" ]
        onAccepted: {
            var path = fileDialog.fileUrl.toString();
            var pre = path.lastIndexOf("file:///");
            if(pre >= 0){
                localUiFile = path.substring(8);
            }
            showBusy();
            var flag = GUiEditor.loadUiFile(localUiFile);
            if(flag){
                loadStatu = true;
            }else{
                messageDialog.open();
                loadStatu = false;
            }
            hideBusy();
        }
    }

    MessageDialog {
        id: messageDialog
        title: "open error"
        text: "Sorry, open project file error."
    }



    //Disable Code
    Button {
        visible: false
        id:bt_publish
        width: windowWidth/20 > 50 ? 50: windowWidth/20
        height: windowWidth/20 > 50 ? 50: windowWidth/20

        opacity: pressed ? themeOpacity : 1
        anchors{
            left:parent.left
            bottom:parent.bottom
            margins: windowWidth > windowHeight  ? windowHeight/20 : windowWidth/20
        }

        style: ButtonStyle {
            background: Image {
                source: "qrc:/images/quit_button.png"
            }
        }

        onClicked: {
            uiLogicalEngine(6,1)
        }
    }
    Button {
        visible: false
        id:bt_globleback
        width:windowWidth/20 > 50 ? 50: windowWidth/20
        height: windowWidth/20 > 50 ? 50: windowWidth/20
        opacity: pressed ? themeOpacity : 1
        anchors{
            right:parent.right
            top:parent.top
            margins: windowWidth > windowHeight  ? windowHeight/20 : windowWidth/20
        }

        style: ButtonStyle {
            background: Image {
                source: "qrc:/images/pick.png"
            }
        }

        onClicked: {
            fileDialog.open();
        }
    }

    Grid{
        visible: false
        signal sig_funcNumber(int funcIndex);
        columns:2
        columnSpacing:windowWidth/8
        rowSpacing: windowHeight/10
        anchors.centerIn: parent
        enabled: loadStatu
        opacity: loadStatu ?1:themeOpacity

        Repeater{
            model: ["多媒体资源更新","界面设计","程序升级","发布"]
            Rectangle{
                color: viceColor
                width: windowWidth/5
                height: windowWidth/5
                radius: index == 3 ? 10: height
                clip:true
                Text{
                    font.family:"微软雅黑"
                    text:modelData
                    //font.family: ""
                    //font.bold: true
                    font.pixelSize: windowWidth/40
                    anchors.centerIn: parent
                    color: "white"
                }
                MouseArea{
                    anchors.fill: parent
                    onPressed: {
                        parent.opacity = themeOpacity
                    }
                    onReleased:  {
                        parent.opacity = 1
                    }
                    onClicked: {
                        uiLogicalEngine(6,index+2)
                    }
                }
            }
        }
    }
}
