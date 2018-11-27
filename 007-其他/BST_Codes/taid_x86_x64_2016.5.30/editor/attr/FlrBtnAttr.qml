import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.2

Item {
    id:root
    anchors.fill: parent
    property var panel: null
    property string elementName: ""
    property int arrortype: 0

    signal sig_attr_resize(int x,int y,int w,int h)

    property string m_background:""
    property string m_bottonOnImg:""
    property string m_bottonOffImg:""
    property string m_invalidFl:""

    property int m_startFlr:0
    property int m_endFlr: m_startFlr + m_rows*m_columes -1;
    property int m_rows:0
    property int m_columes:0
    property alias layoutUD: id_ud.checked
    property alias layoutHV: id_hv.checked
    property string resRootPath:""

    onM_startFlrChanged:realPull()
    onM_rowsChanged: realPull()
    onM_columesChanged: realPull()

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
            spacing: 6
            Text{
                id:testpanel
                width: root.width
                height: root.height/10 > 50 ? 50:root.height/10
                text:elementName
                color: panel.previewModel ? "red" : "white"
                font.pixelSize: height/2
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                MouseArea{
                    anchors.fill: parent
                    onDoubleClicked:{
                        panel.previewModel = !panel.previewModel
                    }
                }

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
                Item{
                    width: parent.width/10
                    height: parent.height
                }

                Text{
                    width: parent.width*2/5
                    height: parent.height
                    text:"height : "
                    color: "white"
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
                    text:"Sync Img: "
                    color: "white"
                    verticalAlignment: Text.AlignVCenter
                }
                Rectangle{
                    width: height
                    height: parent.height
                    color: "transparent"
                    Image {
                        anchors.fill: parent
                        source: "qrc:/images/publish.png"
                    }

                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            ///pullResource();
                            realPull()
                        }
                        z:-10
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
                text:"Effect"
                color: "white"
                font.pixelSize: height/2
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            Row{
                width: root.width
                height:root.height/10 > 30 ? 30 :root.height/10
                anchors.horizontalCenter: parent.horizontalCenter
                Text{
                    width: parent.width/3
                    text:"Rows:"
                    height: parent.height
                    color: "white"
                    font.pixelSize: height/2
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                Row{
                    width: parent.width/2
                    height: parent.height
                    Rectangle{
                        width: parent.width/3
                        height: parent.height
                        TextInput{
                            id:id_rows
                            font.pixelSize: height/2
                            anchors.fill: parent
                            color: "black"
                            maximumLength: 2
                            //inputMask:"99"
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                            onTextChanged: {
                                if(text == ""){
                                    m_rows = 0;
                                    return;
                                }
                                var temp = parseInt(text)
                                if(temp === m_rows){
                                    return;
                                }else if(temp >= 1 && temp <= 50){
                                    m_rows = temp;
                                }else{
                                    text = m_rows;
                                }
                            }
                        }
                    }

                    Text{
                        width:parent.width/3
                        height: parent.height
                        text:" Colume "
                        color: "white"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }

                    Rectangle{
                        width:parent.width/3
                        height: parent.height
                        TextInput{
                            id:id_columes
                            font.pixelSize: height/2
                            anchors.fill: parent
                            color: "black"
                            maximumLength: 2
                            //inputMask:"99"
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                            onTextChanged: {
                                if(text == ""){
                                    m_columes = 0;
                                    return;
                                }
                                var temp = parseInt(text)
                                if(temp === m_columes){
                                    return;
                                }else if(temp >= 1 && temp <= 50){
                                    m_columes = temp;
                                }else{
                                    text = m_columes;
                                }
                            }
                        }
                    }
                }
            }
            Row{
                width: root.width
                height:root.height/10 > 30 ? 30 :root.height/10
                anchors.horizontalCenter: parent.horizontalCenter
                Text{
                    width: parent.width/3
                    text:"Start:"
                    height: parent.height
                    color: "white"
                    font.pixelSize: height/2
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                Row{
                    width: parent.width/2
                    height: parent.height
                    Rectangle{
                        width: parent.width/3
                        height: parent.height
                        TextInput{
                            id:id_start
                            font.pixelSize: height/2
                            anchors.fill: parent
                            color: "black"
                            maximumLength: 2
                            //inputMask:"99"
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                            onTextChanged: {
                                if(text == ""){
                                    m_startFlr = 0;
                                    return;
                                }
                                var temp = parseInt(text)
                                if(temp === m_startFlr){
                                    return;
                                }else if(temp >= -50 && temp <= 100){
                                    m_startFlr = temp;
                                }else{
                                    text = m_startFlr;
                                }
                            }
                        }
                    }

                    Text{
                        width:parent.width/3
                        height: parent.height
                        text:" End "
                        color: "white"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        visible: false
                    }

                    Rectangle{
                        visible: false
                        width:parent.width/3
                        height: parent.height
                        TextInput{
                            id:id_end
                            font.pixelSize: height/2
                            anchors.fill: parent
                            color: "black"
                            maximumLength: 2
                            //inputMask:"99"
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                            onTextChanged: {
                                //m_endFlr = parseInt(text)
                                var temp = parseInt(text)
                                if(m_endFlr == temp){
                                    return;
                                }else if(temp < -50 || temp >100)
                                    text = m_endFlr;
                                else if (temp <= m_startFlr)
                                    text = m_endFlr;
                                else
                                    m_endFlr = temp;
                            }
                        }
                    }
                }
            }
            Row{
                width: root.width
                height:root.height/10 > 30 ? 30 :root.height/10
                anchors.horizontalCenter: parent.horizontalCenter
                Text{
                    width: parent.width/3
                    text:"Invalid Flr: "
                    height: parent.height
                    color: "white"
                    font.pixelSize: height/2
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                Rectangle{
                    width: parent.width/3
                    height: parent.height
                    TextInput{
                        id:id_invalid
                        font.pixelSize: height/2
                        anchors.fill: parent
                        color: "black"
                        //maximumLength: 2
                        //inputMask:"99"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                    }
                }

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
                    height: parent.height/2
                    text:"Up/Down :"
                    color: "white"
                    //horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    anchors.verticalCenter: parent.verticalCenter
                }
                Switch {
                    id:id_ud
                    width: parent.width*3/8 >100 ? 100 : parent.width*3/8
                    height: parent.height/2
                    anchors.verticalCenter: parent.verticalCenter
                    style: SwitchStyle {
                        groove: Rectangle {
                            implicitWidth: control.width
                            implicitHeight: control.height
                            radius: control.height/2
                            color: control.checked ? "green" : "green"
                            Row{
                                anchors.fill: parent
                                Text{
                                    width: control.width/2
                                    height: control.height
                                    text:"BT"
                                    font.pixelSize: height/2
                                    color: "white"
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                }
                                Text{
                                    x:control.width/2
                                    width: control.width/2
                                    height: control.height
                                    text:"TB"
                                    font.pixelSize: height/2
                                    color: "white"
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                }
                            }
                        }
                        handle: Rectangle{
                            implicitWidth: control.width/2
                            implicitHeight: control.height
                            radius: control.height/2
                        }
                    }
                    checked: true
                }
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
                    height: parent.height/2
                    text:"H/V:"
                    color: "white"
                    //horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    anchors.verticalCenter: parent.verticalCenter
                }
                Switch {
                    id:id_hv
                    width: parent.width*3/8 >100 ? 100 : parent.width*3/8
                    height: parent.height/2
                    anchors.verticalCenter: parent.verticalCenter
                    style: SwitchStyle {
                        groove: Rectangle {
                            implicitWidth: control.width
                            implicitHeight: control.height
                            radius: control.height/2
                            color: control.checked ? "green" : "green"
                            Row{
                                anchors.fill: parent
                                Text{
                                    width: control.width/2
                                    height: control.height
                                    text:"H"
                                    font.pixelSize: height/2
                                    color: "white"
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                }
                                Text{
                                    x:control.width/2
                                    width: control.width/2
                                    height: control.height
                                    text:"V"
                                    font.pixelSize: height/2
                                    color: "white"
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                }
                            }
                        }
                        handle: Rectangle{
                            implicitWidth: control.width/2
                            implicitHeight: control.height
                            radius: control.height/2
                        }
                    }
                    checked: true
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
            resRootPath = fileUrl
            realPull();
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

    Component.onCompleted:{

    }

    function initPara(){
        id_rows.text = m_rows
        id_columes.text = m_columes
        id_start.text = m_startFlr
        id_end.text = m_endFlr
        //realPull();
    }

    Timer{
        interval: 500; running: true; repeat: false
        //onTriggered: pullResource();//由FlrBtn面板调用
    }

    function pullResource(){

    }

    function realPull(){
        var proList = GUiEditor.pullProperty(elementName);
        paraProperty(proList);
        initPara();
        if(panel.hasOwnProperty("slt_Resource")){
            var listFlr = GUiEditor.pullFlrBtnResource(m_startFlr,m_endFlr,resRootPath)
            if(listFlr.length > 0){
                resRootPath = listFlr.shift();
                panel.slt_Resource(listFlr);
            }
        }

    }

    function pushResource(){
        if(panel.hasOwnProperty("slt_getRes")){
            var list = panel.slt_getRes();
            GUiEditor.pushFlrBtnResource(Qt.size(m_rows,m_columes),layoutUD,list);
            //console.log("FLRB: ",list)
        }
    }

    function paraProperty(list){
        var i;
        var pair;
        for(i=0; i<list.length;i++){
            pair = list[i].split(":")
            if(pair.length != 2)
                continue;
            switch(pair[0]){
            case "Row":
                m_rows = parseInt(pair[1]);
                break;
            case "Colume":
                m_columes = parseInt(pair[1]);
                break;
            case "StartFlr":
                m_startFlr = parseInt(pair[1]);
                break;
            case "InvaildFlr":
                m_invalidFl = pair[1]
                break;
            case "Flow":
                if(pair[1]=="UP")
                    layoutUD = true;
                else if (pair[1]=="Down")
                    layoutUD = false;
                else{
                    layoutUD = true;
                }

                break;
            case "Direction":
                if(pair[1]=="H")
                    layoutHV = true;
                else if (pair[1]=="V")
                    layoutHV = false;
                else{
                    layoutHV = true;
                }
                break;
            default:
                break;
            }
        }
    }
}
//Row="3" Colume="2" StartFlr="1" InvaildFlr="1,2,3" Flow="Up" Direction="H"


