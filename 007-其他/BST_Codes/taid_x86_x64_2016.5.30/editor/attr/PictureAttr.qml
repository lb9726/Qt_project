import QtQuick 2.3
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2

Item {
    id:root
    anchors.fill: parent
    property var panel: null
    property string elementName: ""
    signal sig_attr_resize(int x,int y,int w,int h)
    property string m_background:""

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
                font.family:"微软雅黑"
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
                    font.family:"微软雅黑"
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
                        //inputMask:"9999"
                        font.family:"微软雅黑"
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
                    font.family:"微软雅黑"
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
                        font.family:"微软雅黑"
                        verticalAlignment: Text.AlignVCenter
                        maximumLength:4
                        //inputMask:"9999"
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
                    font.family:"微软雅黑"
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
                        font.family:"微软雅黑"
                        verticalAlignment: Text.AlignVCenter
                        maximumLength:4
                        //inputMask:"9999"
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
                    font.family:"微软雅黑"
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
                        font.family:"微软雅黑"
                        verticalAlignment: Text.AlignVCenter
                        maximumLength:4
                        //inputMask:"9999"
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
                width: root.width
                height: root.height/10 > 50 ? 50:root.height/10
                text:"Resource"
                font.family:"微软雅黑"
                color: "white"
                font.pixelSize: height/2
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            Item{
                width: root.width
                height:root.height/10 > 50 ? 50 :root.height/10
                Row{
                    height: parent.height
                    anchors.centerIn: parent

                    Button{
                        width: parent.height/2
                        height: parent.height/2
                        anchors.verticalCenter: parent.verticalCenter
                        style: ButtonStyle {
                            background: Image {
                                source: "qrc:/images/add.png"
                            }
                        }
                        opacity: pressed ? themeOpacity : 1
                        onClicked: {
                            fileDialog.open();
                        }
                    }
                    Item {
                        width: parent.width/2
                        height: parent.height/2
                    }
                    Button{
                        width: parent.height/2
                        height: parent.height/2
                        anchors.verticalCenter: parent.verticalCenter
                        style: ButtonStyle {
                            background: Image {
                                source: "qrc:/images/clear.png"
                            }
                        }
                        opacity: pressed ? themeOpacity : 1
                        onClicked: {
                            id_img_model.clear();
                            m_background = ""
                        }
                    }
                }
            }
            GridView{
                id:id_imageView
                width: root.width
                height:(id_img_model.count/3+ (id_img_model.count%3 ? 1:0))* root.width/3
                cellWidth: root.width/3 ; cellHeight: root.width/3
                clip: true
                delegate:Item{
                    width: id_imageView.cellWidth
                    height:width


                    Item{
                        id: video_icon
                        width: id_imageView.cellWidth*3/4; height: id_imageView.cellHeight*3/4
                        anchors {
                            horizontalCenter: parent.horizontalCenter;
                            top: parent.top
                        }

                        Image{
                            //id:resource
                            anchors.fill: parent
                            source: resource
                            z:-1
                            MouseArea{
                                anchors.fill: parent
                                onClicked: selectImage(index)
                            }
                        }
                    }

                    Text{
                        id:id_videoName
                        font.family:"微软雅黑"
                        width: id_imageView.cellWidth;
                        height: id_imageView.cellHeight/4
                        anchors.bottom: parent.bottom
                        text:tipName
                        //font.pointSize: 20;
                        color: "white"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }
                }

                model:id_img_model
            }
        }
    }

    ListModel {
        id: id_img_model
    }

    FileDialog {
        id: fileDialog
        title: "Choose Image"
        //folder: shortcuts.home
        selectMultiple:true
        nameFilters:["Image files (*.jpg *.png)"]
        onAccepted: {
            //id_img_model.clear();
            for(var i = 0;i<fileUrls.length;i++){
                id_img_model.append({'resource':fileUrls[i],'tipName':getFileName(fileUrls[i])})
            }
            m_background = fileUrls[0]
        }
    }

    function getFileName(path){
        var pos = path.lastIndexOf("/");
        if(pos < 0)
            pos = path.lastIndexOf("\\")
        if(pos >= 0)
            return path.substring(pos+1);
        else
            return "";
    }

    function selectImage(index){
        if(index < id_img_model.count){
            m_background = id_img_model.get(index).resource
        }
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
        var res = GUiEditor.pullResource("Picture");
        if(res.length <=0)
            return;
        id_img_model.clear();
        for(var i=0;i<res.length;i++){
            id_img_model.append({'resource':res[i],'tipName':getFileName(res[i])})
        }
        m_background = res[0];
    }

    function pushResource(){
        var list = new Array;
        for(var i =0;i<id_img_model.count;i++){
            list.push(id_img_model.get(i).resource)
        }
        GUiEditor.pushResource("Picture",list);
    }


    //AutoTest

    property int autoFunindex:0

    function initPreview(){
        if(id_img_model.count==0)
            return;
        autoFunindex = 0;
        id_PictureAuto.start();
    }

    function endPreview(){
        id_PictureAuto.stop();
        autoFunindex = 0;
    }

    Timer {
        id:id_PictureAuto
        interval: 5000;
        repeat: true
        onTriggered:{
            playNext();
        }
    }

    function playNext(){
        autoFunindex ++;
        if(autoFunindex + 1 > id_img_model.count){
            autoFunindex = 0
        }
        m_background = id_img_model.get(autoFunindex).resource
    }
}
