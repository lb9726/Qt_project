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
    property real m_volume:50
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
                font.family:"微软雅黑"
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
                    font.family:"微软雅黑"
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
                    text:"y : "
                    font.family:"微软雅黑"
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
                        verticalAlignment: Text.AlignVCenter
                        maximumLength:4
                        font.family:"微软雅黑"
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
                    font.family:"微软雅黑"
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
                            id_video_model.clear();
                            m_background = ""
                        }
                    }
                }
            }
            GridView{
                id:id_videoView
                width: root.width
                height:(id_video_model.count/3+ (id_video_model.count%3 ? 1:0))* root.width/3
                cellWidth: root.width/3 ; cellHeight: root.width/3
                clip: true
                delegate:Item{
                    width: id_videoView.cellWidth
                    height:width
                    Item{
                        id: video_icon
                        width: id_videoView.cellWidth*3/4; height: id_videoView.cellHeight*3/4
                        anchors {
                            horizontalCenter: parent.horizontalCenter;
                            top: parent.top
                        }

                        Image{
                            //id:resource
                            anchors.fill: parent
                            source: "qrc:/images/video.png"
                            z:-1
                            MouseArea{
                                anchors.fill: parent
                                onDoubleClicked: selectVideo(index)
                            }
                        }
                    }

                    Text{
                        id:id_videoName
                        width: id_videoView.cellWidth;
                        height: id_videoView.cellHeight/4
                        anchors.bottom: parent.bottom
                        font.family:"微软雅黑"
                        text:tipName
                        color: "white"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }
                }
                model:id_video_model
            }
            Rectangle{
                width: root.width
                height:2
                opacity: 0.2
            }
            Text{
                width: root.width
                height: root.height/10 > 50 ? 50:root.height/10
                text:"Effect"
                font.family:"微软雅黑"
                color: "white"
                font.pixelSize: height/2
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            Row{
                width: root.width
                height:root.height/10 > 50 ? 50:root.height/10
                Item{
                    width: parent.width/10
                    height: parent.height
                }
                Text{
                    width: parent.width*2/5
                    height: parent.height
                    text:"Volume :"
                    font.family:"微软雅黑"
                    color: "white"
                    //horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                Slider{
                    id:id_volume
                    maximumValue: 100
                    minimumValue: 0
                    stepSize: 1
                    width: parent.width*3/8 >100 ? 100 : parent.width*3/8
                    height: parent.height/2
                    anchors.verticalCenter: parent.verticalCenter
                    //updateValueWhileDragging:false
                    onValueChanged: {
                        if( m_volume != value)
                            m_volume = value;
                    }
                }

                Text{
                    width: parent.width/8
                    height: parent.height
                    text:id_volume.value
                    font.family:"微软雅黑"
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

        }
    }

    ListModel {
        id: id_video_model
    }

    FileDialog {
        id: fileDialog
        title: "Choose Video"
        //folder: shortcuts.home
        selectMultiple:true
        nameFilters:["Image files (*.mp4 *.avi *.rmvb)"]
        onAccepted: {
            for(var i = 0;i<fileUrls.length;i++){
                id_video_model.append({'resource':fileUrls[i],'tipName':getFileName(fileUrls[i])})
            }
            m_background = fileUrls[0]
        }
    }

    Component.onCompleted: {
        id_volume.value = m_volume;
    }

    function selectVideo(index){
        if(index < id_video_model.count){
            if(panel !=null){
                m_background = id_video_model.get(index).resource;
                panel.playVideo();
            }
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
        var res = GUiEditor.pullResource("Video");
        if(res.length <=0)
            return;
        id_video_model.clear();
        for(var i=0;i<res.length;i++){
            id_video_model.append({'resource':res[i],'tipName':getFileName(res[i])});
        }
        m_background = res[0];
        var proList = GUiEditor.pullProperty(elementName);
        paraProperty(proList);
    }

    function pushResource(){
        var list = new Array;
        for(var i =0;i<id_video_model.count;i++){
            list.push(id_video_model.get(i).resource)
        }
        GUiEditor.pushResource("Video",list);

        var prolist = new Array;
        prolist.push("Volume:"+m_volume);
        GUiEditor.pushVideoProperty(prolist);
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

    function paraProperty(list){
        var i;
        var pair;
        for(i=0; i<list.length;i++){
            pair = list[i].split(":")
            if(pair.length != 2)
                continue;
            switch(pair[0]){
            case "Volume":
                m_volume = parseInt(pair[1]);
                id_volume.value = m_volume;
                break;
            default:
                break;
            }
        }
    }
}
