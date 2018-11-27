import QtQuick 2.3
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2
import QtMultimedia 5.5

Item {
    id:root
    anchors.fill: parent
    property var panel: null
    property string elementName: ""
    signal sig_attr_resize(int x,int y,int w,int h)
    property string m_background:"qrc:/images/media.png"

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
                        id:pro_y
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
                    text:"width : "
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

            Rectangle{
                width: root.width
                height:2
                opacity: 0.2
            }
            Text{
                width: root.width
                height: root.height/10 > 50 ? 50:root.height/10
                text:"Resource"
                color: "white"
                font.family:"微软雅黑"
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
                            id_musicModel.clear();
                            m_background = ""
                        }
                    }
                }
            }


            GridView{
                id:id_musicView
                width: root.width
                // height:id_musicModel.count* root.width/9
                height:(id_musicModel.count/3+ (id_musicModel.count%3 ? 1:0))* root.width/3
                cellWidth: root.width/3 ; cellHeight: root.width/3
                clip: true
                delegate:Item{
                    width: id_musicView.cellWidth
                    height:width


                    Item{
                        id: video_icon
                        width: id_musicView.cellWidth*3/4; height: id_musicView.cellHeight*3/4
                        anchors {
                            horizontalCenter: parent.horizontalCenter;
                            top: parent.top
                        }

                        Image{
                            anchors.fill: parent
                            source: "qrc:/images/media.png"
                            z:-1
                            MouseArea{
                                anchors.fill: parent
                                onClicked: selectMusic(index)
                            }
                        }
                    }

                    Text{
                        id:id_videoName
                        width: id_musicView.cellWidth;
                        height: id_musicView.cellHeight/4
                        anchors.bottom: parent.bottom
                        text:tipName
                        //font.pointSize: 20;
                        color: "white"
                        font.family:"微软雅黑"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }
                }

                model:id_musicModel
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
                color: "white"
                font.family:"微软雅黑"
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
                    color: "white"
                    font.family:"微软雅黑"
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
                    color: "white"
                    font.family:"微软雅黑"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

        }
    }

    ListModel {
        id: id_musicModel
    }

    FileDialog {
        id: fileDialog
        title: "Choose Image"
        //folder: shortcuts.home
        selectMultiple:true
        nameFilters:["Image files (*.mp3 *.wav)"]
        onAccepted: {
            //id_musicModel.clear();
            for(var i = 0;i<fileUrls.length;i++){
                id_musicModel.append({'resource':fileUrls[i],'tipName':getFileName(fileUrls[i])})
            }
            m_background = fileUrls[0]
        }
    }

    Component.onCompleted: {
        id_volume.value = m_volume;
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


    function selectMusic(index){
        if(index<id_musicModel.count){
            sampleMusicPlayer.stop();
            sampleMusicPlayer.source = id_musicModel.get(index).resource;
            sampleMusicPlayer.play();
        }
    }
    Audio{
        id: sampleMusicPlayer
        volume: m_volume/100
        onPlaybackStateChanged: {
            if(playbackState ==Audio.PlayingState){

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
        var res = GUiEditor.pullResource("Media");
        if(res.length <=0)
            return;
        id_musicModel.clear();
        for(var i=0;i<res.length;i++){
            id_musicModel.append({'resource':res[i],'tipName':getFileName(res[i])})
        }
        m_background = res[0];

        var proList = GUiEditor.pullProperty(elementName);
        paraProperty(proList);
    }

    function pushResource(){
        var list = new Array;
        for(var i =0;i<id_musicModel.count;i++){
            list.push(id_musicModel.get(i).resource)
        }
        GUiEditor.pushResource("Media",list);

        var prolist = new Array;
        prolist.push("Volume:"+m_volume);
        GUiEditor.pushMediaProperty(prolist);
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
