import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.2
import QtQml.Models 2.1
import QtQuick.Dialogs 1.2
import QtMultimedia 5.5

Item{
    id:root
    property bool pubilshAction: false
    property Item uiEditorPanel:rootWindow.r_uieditor
    property string deviceCount: "0/0"
    property int updateDeviceNum:0
    property bool m_publishing: false
    property string m_failInfo: ""

    Button {
        width:windowWidth/20 > 50 ? 50: windowWidth/20
        height: windowWidth/20 > 50 ? 50: windowWidth/20
        opacity: pressed|| m_publishing ? themeOpacity : 1

        enabled: !m_publishing
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
            uiLogicalEngine(10,0)
        }
    }

    Rectangle{
        width: parent.width*4/5
        height: parent.height*4/5
        anchors.centerIn: parent
        color: viceColor

        Row{
            anchors.fill: parent
            Column{
                id:id_devicelist
                enabled: !m_publishing
                opacity: m_publishing ? themeOpacity : 1

                width: parent.width*2/5
                height: parent.height

                Row{
                    height: id_devicelist.height/8
                    width: id_devicelist.width
                    Text{
                        width: id_devicelist.width/3
                        height: parent.height
                        wrapMode: Text.WrapAnywhere
                        font.pixelSize: height/3
                        //font.bold: true
                        font.family:"微软雅黑"
                        color: "white"
                        text:"Device:"
                        //horizontalAlignment:
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }

                    Text{
                        id:id_onlineDevice
                        width: id_devicelist.width/3
                        height: parent.height
                        wrapMode: Text.WrapAnywhere
                        font.pixelSize: height/3
                        font.bold: true
                        font.family:"微软雅黑"
                        color: "red"
                        text:deviceCount
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }
                    Item{
                        width: id_devicelist.width/3
                        height: parent.height

                        Rectangle {
                            width: height
                            height: parent.height*2/3
                            color: viceColor
                            anchors.centerIn: parent
                            Image{
                                //height: parent.height
                                //height: parent.height
                                anchors.fill: parent
                                source: "qrc:/images/archive.png"
                            }

                            MouseArea{
                                anchors.fill: parent
                                onClicked:refreshDevice();
                                onPressed:parent.opacity = themeOpacity;
                                onReleased: parent.opacity = 1;
                            }
                        }
                    }
                }

                Rectangle{
                    width: parent.width
                    height: 1
                    color: "grey"
                }
                Row{
                    visible: false
                    height: id_devicelist.height/9
                    width: id_devicelist.width
                    Text{
                        width: parent.width/3
                        height: parent.height
                        wrapMode: Text.WrapAnywhere
                        font.pixelSize: height/3
                        font.family:"微软雅黑"
                        color: "white"
                        text:"Model"
                        clip: true
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }
                    Text{
                        width: parent.width*2/3
                        height: parent.height
                        wrapMode: Text.WrapAnywhere
                        font.pixelSize: height/3
                        font.family:"微软雅黑"
                        color:"white"
                        text:"Locate"
                        clip: true
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }
                }
                Rectangle{
                    visible: false
                    width: parent.width
                    height: 1
                    color: "grey"
                }
                ScrollView{
                    style: ScrollViewStyle {
                        transientScrollBars: true
                        handle: Item {
                            implicitWidth: 15
                            implicitHeight: 15
                            Rectangle {
                                color: "white"
                                anchors.fill: parent
                            }
                        }
                        scrollBarBackground: Rectangle {
                            implicitWidth: 15
                            implicitHeight: 15
                            color: mainColor
                        }
                    }
                    width: parent.width
                    height: parent.height*7/8
                    ListView{
                        anchors.fill: parent
                        clip: true
                        model:id_deviceModel

                        delegate: Rectangle{
                            id:id_deviceItem
                            height: id_devicelist.height/8
                            width: id_devicelist.width
                            color: "transparent"
                            Row{
                                anchors.fill: parent
                                Text{
                                    width: parent.width  //3
                                    height: parent.height
                                    wrapMode: Text.WrapAnywhere
                                    font.pixelSize: height/3
                                    font.family:"微软雅黑"
                                    color:select ? "green":"white"
                                    text:"F"+index+"_Terminal"
                                    clip: true
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignHCenter
                                }
                            }
                        }
                        Rectangle{
                            anchors.bottom: parent.bottom
                            width:parent.width
                            height: 1
                            color: "grey"
                        }
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                id_deviceModel.set(index,{"select": (id_deviceModel.get(index).select == 0 ? 1:0) })
                            }
                        }
                    }
                }
            }


            Rectangle{
                width: 2
                height: parent.height
            }

            Column{
                width: parent.width*3/5
                height: parent.height
                clip: true
                Item{
                    width: parent.width
                    height: parent.height/3
                    visible: !m_publishing
                }

                Item{
                    width: parent.width
                    height: parent.height/3
                    opacity: m_publishing ? themeOpacity : 1
                    enabled: !m_publishing
                    Button{
                        anchors.centerIn: parent
                        width: id_devicelist.width*2/3
                        height: parent.height*2/3

                        Text{
                            text: "Publish"
                            font.family:"微软雅黑"
                            font.pixelSize: parent.height/3
                            anchors.centerIn: parent
                            //horizontalAlignment: Text.AlignHCenter
                            //verticalAlignment: Text.AlignVCenter
                            //mainColor
                        }

                        style: ButtonStyle {
                            background: Rectangle {
                                implicitWidth: 100
                                implicitHeight: 25
                                border.width: control.activeFocus ? 2 : 1
                                border.color: "#888"
                                radius: 4
                                gradient: Gradient {
                                    GradientStop { position: 0 ; color: control.pressed ? "#ccc" : "#eee" }
                                    GradientStop { position: 1 ; color: control.pressed ? "#aaa" : "#ccc" }
                                }
                            }
                        }
                        onClicked: {
                            publishElement();
                        }
                    }
                    Behavior on y {
                         NumberAnimation { duration: 1000 }
                    }
                }

                Item{
                    visible: m_publishing
                    width: parent.width
                    height: parent.height/6
                    Column{
                        opacity: 0.6
                        anchors.fill: parent
                        spacing: height/4
                        Row{
//                            visible: {
//                                if(updateDeviceNum > 1)
//                                    return true;
//                                else
//                                    return false;
//                            }
                            visible:false
                            width: parent.width
                            height: parent.height/3
                            ProgressBar{
                                id:id_toCount
                                maximumValue:updateDeviceNum
                                minimumValue:0
                                width: parent.width*5/6
                                height: parent.height
                                value: 0
                            }
                            Text{
                                width: parent.width/6
                                height: parent.height
                                text: id_toCount.value
                                color: "white"
                                font.family:"微软雅黑"
                                //font.pixelSize: height/2
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                        }

                        Row{
                            id:id_progressArea
                            //visible: m_publishing
                            width: parent.width
                            height: parent.height/3
                            Item{
                                width: parent.width*5/6
                                height: parent.height
                                //visible: m_publishing
                                ProgressBar{
                                    id:id_proCount
                                    maximumValue:100
                                    minimumValue:0
                                    width: parent.width*2/3
                                    height: parent.height*2/3
                                    anchors.centerIn: parent
                                    value: 0
                                    visible: m_publishing
                                }
                            }

                            Text{
                                width: parent.width/6
                                height: parent.height
                                text: id_proCount.value
                                color: "white"
                                font.family:"微软雅黑"
                                font.pixelSize: height/2
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                //visible: m_publishing
                            }
                        }
                    }
                }

                Rectangle{
                    visible: m_publishing
                    width: parent.width
                    height: parent.height/2
                    TextArea{
                        id:id_tipInfo
                        enabled: false
                        anchors.fill: parent
                        textColor:"black"
                        backgroundVisible: true
                        font.family:"微软雅黑"
                        font.pixelSize: height/16
                        //horizontalAlignment: Text.AlignHCenter
                        //verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }
    }

    ListModel{
        id:id_deviceModel
        //ListElement{devID:"BVT510";select:0}
        //ListElement{devID:"BVT510";select:0}
    }

    function refreshDevice(){
        id_deviceModel.clear();
        var devid,i;
        var terminallist = Publisher.getTerminalList();

        for(i=0;i<terminallist.length;i++){

            devid = terminallist[i].split("_");
            if(devid.length != 2)
                continue;
            //console.log(devid[1],rootWindow.deviceModel);
            //if(devid[1] == rootWindow.deviceModel){
            id_deviceModel.append({"devID": terminallist[i], "select":0})
            //}
        }
        deviceCount = ""+id_deviceModel.count+"/"+terminallist.length;
    }

    Connections{
        target: rootWindow
        onSpublishProgress:{
            if(deviceOrder<=updateDeviceNum){
                updateDeviceNum = deviceOrder;
            }
            if(proValue<=100){
                id_proCount.value = proValue
            }
        }
        onSpublishError:{
            m_failInfo = info;
            failAction();
        }
        onSpublishFinish:{
            finishAction();
        }
    }

    Connections{
        target: rootWindow
        onScompressProgress:{
            if(proValue<=100){
                id_proCount.value = proValue
            }
        }
        onScompressfinish:{

            publishResource()
        }
    }

    Component.onCompleted: {
        refreshDevice();
    }

    property var upDatelist: new Array

    function getUpdateDevice(){
        upDatelist = new Array;
        for(var i=0;i<id_deviceModel.count;i++){
            var devItem = id_deviceModel.get(i)
            if(devItem.select == 1){
                upDatelist.push(devItem.devID);
            }
        }

        updateDeviceNum = upDatelist.length;

        //console.log(upDatelist)
    }

    function publishElement(){
        startAction();
        getUpdateDevice();
        if(updateDeviceNum<=0){
            m_failInfo = "Please select the devie to publish";
            failAction();
            return;
        }
        pushTip("Saving project file ... ");
        if(uiEditorPanel !== null){
            uiEditorPanel.publishCtrl();
        }
        showBusy();
        GUiEditor.savePara();
        GUiEditor.orderResouce();
        hideBusy();
        pushTip("Archiving project file ... ");
        GUiEditor.archiveCurTheme();
    }

    function publishResource(){
        pushTip("Archive Finish.");
        pushTip("Start Publish\nUploading ... ");
        Publisher.batchUpdate(upDatelist);
        //finishAction();
    }

    function pushTip(info){
        var old = id_tipInfo.text;
        id_tipInfo.text = old +"\n"+ info
    }
    function clearTip(){
        id_tipInfo.text = "";
    }

    function startAction(){
        id_proCount.value = 0
        m_publishing = true;
        clearTip();
    }

    function failAction(){
        pushTip(m_failInfo);
        id_proCount.value = 100;
        id_triggleTimer.start();
        updateDeviceNum = 0;
        for(var i=0;i<id_deviceModel.count;i++){
            id_deviceModel.set(i,{"select":0})
        }
    }

    function finishAction(){
        pushTip("Publish Finish.");
        id_proCount.value =100
        id_triggleTimer.start();
        updateDeviceNum = 0;
        for(var i=0;i<id_deviceModel.count;i++){
            id_deviceModel.set(i,{"select":0})
        }
    }


    Timer{
        id:id_triggleTimer
        interval: 1000
        repeat: false
        onTriggered: {
            ///console.log("id_triggleTimer")
            id_proCount.value = 0
            m_publishing = false;
            clearTip();
        }
    }
}
