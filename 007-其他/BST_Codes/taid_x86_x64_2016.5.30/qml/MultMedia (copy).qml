import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.2
import QtQml.Models 2.1
import QtQuick.Dialogs 1.2

Item{
    id:root
    //anchors.fill: parent
    property int tabindex: 0

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
            uiLogicalEngine(7,0)
        }
    }

    Button {
        id:bt_publish
        width: windowWidth/20 > 50 ? 50: windowWidth/20
        height: windowWidth/20 > 50 ? 50: windowWidth/20

        opacity: pressed ? themeOpacity : 1

        anchors{
            right:parent.right
            top:parent.top
            margins: windowWidth > windowHeight  ? windowHeight/20 : windowWidth/20
        }

        style: ButtonStyle {
            background: Image {
                source: "qrc:/images/publish.png"
            }

        }

        onClicked: {
            uiLogicalEngine(7,1)
        }
    }

    Column{
        width:parent.width*4/5
        height: parent.height*5/6
        anchors.centerIn: parent

        Column{
            width: parent.width
            height: parent.height/6

            Row{
                width: parent.width
                height: parent.height*3/4

                Item{
                    width: parent.width/3
                    height: parent.height
                    Rectangle{
                        anchors.fill: parent
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        color: viceColor
                        Text{
                            text:"图片"
                            color: "white"
                            font.pixelSize: parent.height/4
                            anchors.centerIn: parent
                        }
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                multmedia_OperateLoader.sourceComponent = ad_panel
                                tabindex = 0
                            }

                        }
                    }
                }

                Item{
                    width: parent.width/3
                    height: parent.height
                    Rectangle{
                        anchors.fill: parent
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        color: viceColor

                        Text{
                            text:"视频"
                            color: "white"
                            font.pixelSize: parent.height/4
                            anchors.centerIn: parent
                        }
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                multmedia_OperateLoader.sourceComponent = video_panel
                                tabindex = 1
                            }

                        }
                    }
                }

                Item{
                    width: parent.width/3
                    height: parent.height
                    Rectangle{
                        anchors.fill: parent
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        color: viceColor
                        Text{
                            text:"背景音乐"
                            color: "white"
                            font.pixelSize: parent.height/4
                            anchors.centerIn: parent
                        }

                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                multmedia_OperateLoader.sourceComponent = music_panel
                                tabindex = 2
                            }
                        }
                    }
                }
            }

            Item{
                width: 30
                height: parent.height/4
                Rectangle{
                    width: parent.width/2
                    height: parent.height/2
                    anchors.centerIn: parent
                }
                x:parent.width*(2*tabindex+1)/6-width/2
                Behavior on x{ SpringAnimation {spring: 2; damping: 0.2 }}
            }
        }

        Loader{
            id:multmedia_OperateLoader
            width: parent.width
            height: parent.height*5/6
        }
    }

    Column{
        anchors.verticalCenter: parent.verticalCenter
        anchors.right:  parent.right
        anchors.rightMargin: windowWidth/40
        spacing: windowWidth/20
        Button {
            width: windowWidth/20 > 50 ? 50: windowWidth/20
            height: windowWidth/20 > 50 ? 50: windowWidth/20
            opacity: pressed ? themeOpacity : 1

            style: ButtonStyle {
                background: Image {
                    source: "qrc:/images/add.png"
                }
            }

            onClicked: {
                //nameFilter = "Image files (*.jpg *.png)", "All files (*)";
                mediaOperat(1)
            }
        }

        Button {
            width: windowWidth/20 > 50 ? 50: windowWidth/20
            height: windowWidth/20 > 50 ? 50: windowWidth/20
            opacity: pressed ? themeOpacity : 1


            style: ButtonStyle {
                background: Image {
                    source: "qrc:/images/back.png"
                }
            }

            onClicked: {
                mediaOperat(2)
            }
        }
        Button {
            width: windowWidth/20 > 50 ? 50: windowWidth/20
            height: windowWidth/20 > 50 ? 50: windowWidth/20
            opacity: pressed ? themeOpacity : 1


            style: ButtonStyle {
                background: Image {
                    source: "qrc:/images/clear.png"
                }
            }

            onClicked: {
                mediaOperat(3)
            }
        }
    }

    Component.onCompleted: {
        multmedia_OperateLoader.sourceComponent = ad_panel
    }
    property int ad_currentIndex: -1

    Component{
        id:ad_panel
        Rectangle{
            anchors.fill: parent
            clip:true
            color: viceColor
            Text{
                text:"请添加图片"
                color: "white"
                font.pixelSize: parent.height/12
                anchors.centerIn: parent

                visible: adpicture_panel.count >0 ? false : true
            }

            Component{
                id:dragDelegate
                Rectangle {
                    id: icon
                    width: adpicture_panel.cellWidth
                    height: adpicture_panel.cellHeight
                    color: "transparent"
                    border.color: "red"
                    border.width: 2
                    Row{
                        anchors.centerIn:  parent
                        anchors.margins: 6
                        spacing: 10

                        Item{
                            width: Math.min(icon.height,icon.width/2)
                            height: Math.min(icon.height,icon.width/2)
                            //anchors.verticalCenter: parent.verticalCenter
                            Image {
                                anchors.centerIn:  parent
                                width: parent.height*3/4;
                                height: parent.height*3/4;
                                source: model.img
                                visible: ok_button.visible ? false : true
                                MouseArea{
                                    anchors.fill: parent
                                    onClicked: {
                                        console.log("repleace img");
                                    }
                                    onPressAndHold: {
                                        console.log("delete img");
                                    }
                                }
                            }

                            Rectangle{
                                id:ok_button
                                width: parent.width*3/4
                                height: parent.height/2
                                anchors.centerIn: parent
                                color: viceColor
                                radius: 4
                                Text {
                                    text: qsTr("确定")
                                    anchors.centerIn: parent
                                    font.pixelSize: parent.height/4
                                    color: "white"
                                }
                                z:1
                                visible: orderID.enabled ? true : false
                                MouseArea{
                                    anchors.fill: parent
                                    onClicked: {
                                        orderID.enabled = false
                                    }
                                    onPressed: {
                                        parent.opacity = themeOpacity
                                    }
                                    onReleased: {
                                        parent.opacity = 1
                                    }
                                }
                            }
                        }

                        Item{
                            width: icon.width/2
                            height: icon.height
                            anchors.verticalCenter: parent.verticalCenter
                            clip: true
                            TextInput {
                                anchors.fill: parent
                                id: orderID
                                text: qsTr(model.indexNumber)
                                maximumLength: 5
                                enabled: false
                                font.pixelSize: parent.height/2
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                color:enabled ? "red":"white"
                                //focus:true
                                Keys.onEnterPressed: {
                                    orderID.enabled = false
                                }
                            }
                        }
                    }

                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            ad_currentIndex = adpicture_panel.currentIndex = index
                        }

                        onDoubleClicked: {
                            orderID.enabled = true
                            z = -1
                        }
                        onPressAndHold: {
                            orderID.enabled = true
                            z = -1
                        }
                        z:orderID.enabled ? -1 :10
                    }
                }
            }

            DelegateModel {
                id: visualModel

                function lessThan (left, right) {
                    console.log(left.indexNumber , right.indexNumber);
                    return left.indexNumber > right.indexNumber
                }

                function insertPosition( item) {
                    var lower = 0
                    var upper = items.count
                    while (lower < upper) {
                        var middle = Math.floor(lower + (upper - lower) / 2)
                        var result = lessThan(item.model, items.get(middle).model);
                        if (result) {
                            upper = middle
                        } else {
                            lower = middle + 1
                        }
                    }
                    return lower
                }

                function sort() {
                    ///while (items.count > 0) {
                    for(var i = 0;i<items.count;i++){
                        var item = items.get(i)
                        var index = insertPosition(item)

                        //item.groups = "items"
                        items.move(item.itemsIndex, index)
                    }
                }

                items.includeByDefault: true
                model: ad_image_model
                delegate: dragDelegate
            }

            GridView {
                id:adpicture_panel
                anchors.fill: parent
                // visible: false
                cellWidth: parent.width/3
                cellHeight: parent.height/4

                model: visualModel
                //highlightRangeMode:GridView.StrictlyEnforceRange
                highlightFollowsCurrentItem:false
                highlight: Rectangle {
                    opacity: 0.2
                    width: adpicture_panel.cellWidth; height: adpicture_panel.cellHeight
                    color: "red"; radius: 5
                    border.color: "white"
                    border.width: 2
                    x: adpicture_panel.currentItem.x
                    y: adpicture_panel.currentItem.y
                    Behavior on x { SpringAnimation { spring: 3; damping: 0.4 } }
                    Behavior on y { SpringAnimation { spring: 3; damping: 0.4 } }
                }
            }
        }
    }

    Component{
        id:video_panel
        Rectangle{
            color: "green"
            opacity: themeOpacity
        }
    }

    Component{
        id:music_panel
        Rectangle{
            color: "red"
            opacity: themeOpacity
        }
    }

    ListModel {
        id: ad_image_model
    }

    WorkerScript {
        id: myWorker
        source: "qrc:/js/Worker.js"
        //onMessage: myText.text = messageObject.reply
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        folder: shortcuts.home
        selectMultiple:true
        nameFilters: [ ]
        onAccepted: {
            var i = 0;
            var fileArray = new Array;
            if(tabindex == 0){
                fileArray[0] = {'action': ad_image_model, "index":"x"}
                for( i=0;i<fileDialog.fileUrls.length;i++)
                {
                    fileArray[i+1] = {'action': fileDialog.fileUrls[i],"index":"xx"}
                }

                myWorker.sendMessage(fileArray)
            }

            if(tabindex == 1){
                fileArray[0] = {'action': ad_image_model, "index":"x"}
                for( i=0;i<fileDialog.fileUrls.length;i++)
                {
                    fileArray[i+1] = {'action': fileDialog.fileUrls[i],"index":"xx"}
                }

                myWorker.sendMessage(fileArray)
            }

            if(tabindex == 2){
                fileArray[0] = {'action': ad_image_model, "index":"x"}
                for( i=0;i<fileDialog.fileUrls.length;i++)
                {
                    fileArray[i+1] = {'action': fileDialog.fileUrls[i],"index":"xx"}
                }

                myWorker.sendMessage(fileArray)
            }
        }

        onRejected: {
            console.log("Canceled")
        }
    }

    property string nameFilter: ""

    function mediaOperat(op_type){
        switch(op_type){
        case 1:
            addmedia();
            break;
        case 2:
            deletemedia();
            break;
        case 3:
            break;
        default:
            return;
        }
    }

    function addmedia(){
        if(tabindex == 0)
            fileDialog.nameFilters = ["Image files (*.jpg *.png)", "All files (*)"]
        else if(tabindex == 1)
            fileDialog.nameFilters = ["Image files (*.mp4 *.rmvb)", "All files (*)"]
        else if(tabindex == 2)
            fileDialog.nameFilters = ["Image files (*.wav *.mp3)", "All files (*)"]
        else
            return
        fileDialog.open();
    }

    function deletemedia(){
        if(tabindex == 0 && ad_currentIndex >= 0){
            ad_image_model.remove(ad_currentIndex);
        }else if(tabindex == 1){
            //fileDialog.nameFilters = ["Image files (*.mp4 *.rmvb)", "All files (*)"]
        }else if(tabindex == 2){
            //fileDialog.nameFilters = ["Image files (*.wav *.mp3)", "All files (*)"]
        }else
            return
    }

    function clearmedia(){
        if(tabindex == 0 ){
            ad_image_model.clear();
        }else if(tabindex == 1){

        }else if(tabindex == 2){

        }else
            return
    }
}
