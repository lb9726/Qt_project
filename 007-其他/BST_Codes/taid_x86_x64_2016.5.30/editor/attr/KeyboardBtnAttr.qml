import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2
Item {
    id:root
    anchors.fill: parent
    property var panel: null
    property string elementName: ""
    signal sig_attr_resize(int x,int y,int w,int h)
    property string m_background: ""
    property string resRootPath:""

    ScrollView {
        anchors.fill: parent
        horizontalScrollBarPolicy:Qt.ScrollBarAlwaysOff
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
        Column{
            id:base_area
            spacing: 4
            Text{
                id:testpanel
                width: root.width
                height: root.height/10 > 50 ? 50:root.height/10
                text:elementName
                color: "white"
                font.pixelSize: height/2
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            Rectangle{
                width: root.width
                height:2
                opacity: 0.2
            }

            Row{
                width: root.width
                height:40
                Item{
                    width: parent.width/10
                    height: parent.height
                }

                Text{
                    width: parent.width*2/5
                    height: parent.height
                    text:"x : "
                    color: "white"
                    //horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                Rectangle{
                    width: parent.width*2/5 -10
                    height: 30
                    anchors.verticalCenter: parent.verticalCenter
                    TextInput{
                        id:pro_x
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        maximumLength:4
                        inputMask:"9999"
                        Keys.onEnterPressed: {
                            paraChange();
                        }
                        onFocusChanged: {
                            paraChange();
                        }
                    }
                }
            }
            Row{
                width: root.width
                height:40
                //spacing: 10
                Item{
                    width: parent.width/10
                    height: parent.height
                }
                Text{
                    width: parent.width*2/5
                    height: parent.height
                    text:"y : "
                    color: "white"
                    //horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                Rectangle{
                    width: parent.width*2/5 -10
                    height: 30
                    anchors.verticalCenter: parent.verticalCenter
                    TextInput{
                        id:pro_y
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        maximumLength:4
                        inputMask:"9999"
                        Keys.onEnterPressed: {
                            paraChange();
                        }
                        onFocusChanged: {
                            paraChange();
                        }
                    }
                }
            }
            Row{
                width: root.width
                height:40
                //spacing: 10
                Item{
                    width: parent.width/10
                    height: parent.height
                }
                Text{
                    width: parent.width*2/5
                    height: parent.height
                    text:"width : "
                    color: "white"
                    //horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                Rectangle{
                    width: parent.width*2/5 -10
                    height: 30
                    anchors.verticalCenter: parent.verticalCenter
                    TextInput{
                        id:pro_width
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        maximumLength:4
                        inputMask:"9999"
                        Keys.onEnterPressed: {
                            paraChange();
                        }
                        onFocusChanged: {
                            paraChange();
                        }
                    }
                }
            }
            Row{
                width: root.width
                height:40
                //spacing: 10
                Item{
                    width: parent.width/10
                    height: parent.height
                }
                Text{
                    width: parent.width*2/5
                    height: parent.height
                    text:"height : "
                    color: "white"
                    //horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                Rectangle{
                    width: parent.width*2/5 -10
                    height: 30
                    anchors.verticalCenter: parent.verticalCenter

                    TextInput{
                        id:pro_height
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        maximumLength:4
                        inputMask:"9999"
                        Keys.onEnterPressed: {
                            paraChange();
                        }
                        onFocusChanged: {
                            paraChange();
                        }
                    }
                }
            }


            Rectangle{
                width: root.width
                height:2
                opacity: 0.2
            }
            Text{
                //id:testpanel
                width: root.width
                height: root.height/10 > 50 ? 50:root.height/10
                text:"Resuorce"
                color: "white"
                font.pixelSize: height/2
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            Row{
                width: root.width
                height:root.height/10
                Item{
                    width: parent.width/10
                    height: parent.height
                }
                Text{
                    width: parent.width*2/5
                    height: parent.height
                    text:"Pick Img: "
                    color: "white"
                    //horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                Rectangle{
                    width: height
                    height: parent.height
                    color: "transparent"
                    border.width: id_backImg.source == "" ? 1 : 0
                    border.color: "white"
                    Image {
                        id: id_backImg
                        anchors.fill: parent
                        source: "qrc:/images/add.png"
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            selectImg(1)
                        }
                        z:-10
                    }
                }
            }
        }
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a folder"
        //folder: shortcuts.home
        selectFolder: true
        onAccepted: {
            if(panel.hasOwnProperty("slt_Resource")){
                var listFlr = GUiEditor.scanKeyBoardImg(fileUrl);
                if(listFlr.length > 0){
                    resRootPath = listFlr.shift();
                    panel.slt_Resource(listFlr);
                }
            }
        }

        onRejected: {
            pushResource();
        }
    }

    function selectImg(index){
        fileDialog.selectMultiple = false;
        fileDialog.open();
    }

    function paraChange(){
        var pX = pro_x.text
        var pY = pro_y.text
        var pWidth = pro_width.text
        var pHeight = pro_height.text
        if(pX==0||pY==0||pWidth==0||pHeight==0){
            return;
        }else{
            sig_attr_resize(pX,pY,pWidth,pHeight);
        }
    }

    function setcord(xcord,ycord){
        pro_x.text = xcord
        pro_y.text = ycord
    }

    function setsize(w,h){
        pro_width.text = w
        pro_height.text = h
    }

    Timer {
        interval: 1000; running: true; repeat: false
        //onTriggered: pullResource()
    }

    function pullResource(){
        if(panel.hasOwnProperty("slt_Resource")){
            var listFlr = GUiEditor.pullResource(elementName);
            if(listFlr.length > 0){
                resRootPath = listFlr.shift();
                panel.slt_Resource(listFlr);
            }
        }
    }

    function pushResource(){
        if(panel.hasOwnProperty("slt_getRes")){
            var list = panel.slt_getRes();
            GUiEditor.pushResource(elementName,list);
        }
    }
}
