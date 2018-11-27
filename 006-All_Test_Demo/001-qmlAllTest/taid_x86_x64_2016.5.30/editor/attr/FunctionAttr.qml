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
    property string m_tipText: ""

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
                    font.family:"微软雅黑"
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
                    font.family:"微软雅黑"
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
                            selectfuncResource();
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
                            clearfunImg();
                        }
                    }
                }
            }

            GridView{
                id:id_funLogoImgList
                width: root.width
                height:id_funLogoModel.count* root.width/9
                cellWidth: root.width/3 ; cellHeight: root.width/3
                clip: true
                delegate:Item{
                    width: id_funLogoImgList.cellWidth
                    height:width
                    Image{
                        anchors.fill:parent
                        anchors.margins: parent.width/10
                        source: imgPath
                        MouseArea{
                            anchors.fill: parent
                            onClicked: selectFuncLogo(index)
                        }
                    }
                }
                model:id_funLogoModel
            }
        }
    }

    function editeEnd(){
        id_funNumber.text = id_funNumberIn.text
        id_funNumber.visible = true
    }

    ListModel{
        id:id_funLogoModel
        //ListModel{fileName:"",imgPath:"",desc:""}
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        nameFilters: [ "Image files (*.jpg *.png)"]
        //folder: shortcuts.home
        selectMultiple: true
        onAccepted: {
            for(var i = 0;i<fileUrls.length;i++){
                var fileName = getFileName(fileUrls[i]);
                var pValue = parseInt(fileName)
                if((NaN != pValue) && (pValue >0) && (pValue< 999)){
                    id_funLogoModel.append({fileName:fileName,imgPath:fileUrls[i],desc:""})
                }
            }
        }

        onRejected: {

        }
    }

    function getFileName(path){
        return path.substring(path.lastIndexOf("/")+1,path.lastIndexOf("."));
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

    function selectfuncResource(){
        fileDialog.open();
    }

    function clearfunImg(){
        id_funLogoModel.clear()
    }

    Timer {
        interval: 1000; running: true; repeat: false
        //onTriggered: pullResource()
    }


    function selectFuncLogo(index){
        if(index < id_funLogoModel.count){
            m_background = id_funLogoModel.get(index).imgPath
            m_tipText = id_funLogoModel.get(index).desc
        }
    }

    function pullResource(){
        var funcArray;
        var fileName;
        var pValue;
        var i;
        var res = GUiEditor.pullResource(elementName);
        id_funLogoModel.clear();
        for(i = 0;i<res.length;i++){
            funcArray = res[i].split("@");
            fileName = getFileName(funcArray[0]);
            pValue = parseInt(fileName)
            if((NaN != pValue) && (pValue >0) && (pValue< 999)){
                id_funLogoModel.append({fileName:fileName,imgPath:rootWindow.filePrefix + funcArray[0],desc:funcArray[1]})
            }
        }
        selectFuncLogo(0);
    }

    function pushResource(){
        var list = new Array;
        var tmp,i;
        for(i=0;i<id_funLogoModel.count;i++){
            tmp = id_funLogoModel.get(i);
            list.push(tmp.imgPath+"@"+tmp.desc);
        }
        GUiEditor.pushResource(elementName,list);
    }

    property int autoFunindex:0

    function initPreview(){
        if(id_funLogoModel.count==0)
            return;
        autoFunindex = 0;
        id_funcAuto.start();
    }

    function endPreview(){
        id_funcAuto.stop();
        autoFunindex = 0;
    }

    Timer {
        id:id_funcAuto
        interval: 5000;
        repeat: true
        onTriggered:{
            playNext();
        }
    }

    function playNext(){
        autoFunindex ++;
        if(autoFunindex + 1 > id_funLogoModel.count){
            autoFunindex = 0
        }
        m_background = id_funLogoModel.get(autoFunindex).imgPath
        m_tipText = id_funLogoModel.get(autoFunindex).desc
    }
}
