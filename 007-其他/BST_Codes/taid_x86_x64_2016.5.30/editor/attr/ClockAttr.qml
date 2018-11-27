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
    property alias m_ShowSecond:id_isShowSecond.checked
    property string m_DateStyle:""
    property alias m_editable:id_editable.checked

    property alias m_dx:pro_dx.text
    property alias m_dy:pro_dy.text
    property alias m_dh:pro_dh.text
    property alias m_dw:pro_dw.text

    property alias m_tx:pro_tx.text
    property alias m_ty:pro_ty.text
    property alias m_th:pro_th.text
    property alias m_tw:pro_tw.text

    property string dResPath:""
    property string tResPath:""

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
                    text:"Background: "
                    color: "white"
                    //horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                Rectangle{
                    width: height
                    height: parent.height
                    color: "transparent"
                    border.width: 1
                    border.color: "white"
                    Image {
                        anchors.fill: parent
                        source:m_background
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
                    text:"Date : "
                    color: "white"
                    verticalAlignment: Text.AlignVCenter
                }
                Image {
                    width: height
                    height: parent.height
                    source: "qrc:/images/add.png"
                    MouseArea{
                        anchors.fill: parent
                        onDoubleClicked:  {
                            selectImg(2)
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
                    text:"Time: "
                    color: "white"
                    //horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                Image {
                    width: height
                    height: parent.height
                    source: "qrc:/images/add.png"
                    MouseArea{
                        anchors.fill: parent
                        onDoubleClicked:  {
                            selectImg(3)
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
                text:"Edit"
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
                    height: parent.height/2
                    text:"Edit:"
                    color: "white"
                    //horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    anchors.verticalCenter: parent.verticalCenter
                }

                Switch {
                    id:id_editable
                    width: parent.width*3/8 >100 ? 100 : parent.width*3/8
                    height: parent.height/2
                    anchors.verticalCenter: parent.verticalCenter
                    style: SwitchStyle {
                        groove: Rectangle {
                            implicitWidth: control.width
                            implicitHeight: control.height
                            radius: control.height/2
                            color: control.checked ? "green" : "gray"
                            Row{
                                anchors.fill: parent
                                Text{
                                    width: control.width/2
                                    height: control.height
                                    text:"ON"
                                    font.pixelSize: height/2
                                    color: "white"
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                }
                                Text{
                                    x:control.width/2
                                    width: control.width/2
                                    height: control.height
                                    text:"OFF"
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
                    checked: false
                }
            }

            Rectangle{
                width: root.width
                height:2
                opacity: 0.2
                visible: m_editable
            }
            Text{
                visible: m_editable
                width: root.width
                height: root.height/10 > 50 ? 50:root.height/10
                text:"Date"
                color: "white"
                font.pixelSize: height/2
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            Row{
                visible: m_editable
                opacity:themeOpacity
                enabled: false
                width: root.width
                height:40
                Item{
                    width: parent.width/10
                    height: parent.height
                }

                Text{
                    width: parent.width*2/5
                    height: parent.height
                    text:"x , y : "
                    color: "white"
                    //horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                Rectangle{
                    width: parent.width*1/10
                    height: 30
                    anchors.verticalCenter: parent.verticalCenter
                    Text{
                        id:pro_dx
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                    }
                }
                Text{
                    width: parent.width*1/10
                    height: parent.height
                    text:"-"
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                Rectangle{
                    width: parent.width*1/10
                    height: 30
                    anchors.verticalCenter: parent.verticalCenter
                    Text{
                        id:pro_dy
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
            Row{
                visible: m_editable
                opacity:themeOpacity
                enabled: false
                width: root.width
                height:40
                Item{
                    width: parent.width/10
                    height: parent.height
                }

                Text{
                    width: parent.width*2/5
                    height: parent.height
                    text:"width , height : "
                    color: "white"
                    //horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                Rectangle{
                    width: parent.width*1/10
                    height: 30
                    anchors.verticalCenter: parent.verticalCenter
                    Text{
                        id:pro_dw
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                    }
                }
                Text{
                    width: parent.width*1/10
                    height: parent.height
                    text:"-"
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                Rectangle{
                    width: parent.width*1/10
                    height: 30
                    anchors.verticalCenter: parent.verticalCenter
                    Text{
                        id:pro_dh
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
            Row{
                visible: m_editable
                width: root.width
                height:root.height/10 > 50 ? 50 : root.height/10
                Item{
                    width: parent.width/10
                    height: parent.height
                }
                Text{
                    width: parent.width*2/5
                    height: parent.height
                    text:"Date Format: "
                    color: "white"
                    //horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                Rectangle{
                    width: parent.width/2 >120 ? 120 : parent.width/2
                    height: parent.height/2
                    color: viceColor
                    anchors.verticalCenter: parent.verticalCenter
                    ComboBox {
                        anchors.fill: parent
                        model: [ "yyyy-MM-dd", "yyyy-dd-MM"]
                        style: ComboBoxStyle{
                            background:Item{}
                            textColor:"white"
                        }
                        editable:false
                        onCurrentTextChanged: {
                            m_DateStyle = currentText;
                        }
                    }
                }
            }

            Rectangle{
                visible: m_editable
                width: root.width
                height:2
                opacity: 0.2
            }
            Text{
                visible: m_editable
                width: root.width
                height: root.height/10 > 50 ? 50:root.height/10
                text:"Time"
                color: "white"
                font.pixelSize: height/2
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            Row{
                visible: m_editable
                opacity:themeOpacity
                enabled: false
                width: root.width
                height:40
                Item{
                    width: parent.width/10
                    height: parent.height
                }

                Text{
                    width: parent.width*2/5
                    height: parent.height
                    text:"x , y : "
                    color: "white"
                    //horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                Rectangle{
                    width: parent.width*1/10
                    height: 30
                    anchors.verticalCenter: parent.verticalCenter
                    Text{
                        id:pro_tx
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                    }
                }
                Text{
                    width: parent.width*1/10
                    height: parent.height
                    text:"-"
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                Rectangle{
                    width: parent.width*1/10
                    height: 30
                    anchors.verticalCenter: parent.verticalCenter
                    Text{
                        id:pro_ty
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
            Row{
                visible: m_editable
                opacity:themeOpacity
                enabled: false
                width: root.width
                height:40
                Item{
                    width: parent.width/10
                    height: parent.height
                }

                Text{
                    width: parent.width*2/5
                    height: parent.height
                    text:"width , height : "
                    color: "white"
                    //horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                Rectangle{
                    width: parent.width*1/10
                    height: 30
                    anchors.verticalCenter: parent.verticalCenter
                    Text{
                        id:pro_tw
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                    }
                }
                Text{
                    width: parent.width*1/10
                    height: parent.height
                    text:"-"
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                Rectangle{
                    width: parent.width*1/10
                    height: 30
                    anchors.verticalCenter: parent.verticalCenter
                    Text{
                        id:pro_th
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
            Row{
                visible: m_editable
                width: root.width
                height:root.height/10 > 50 ? 50 : root.height/10
                Item{
                    width: parent.width/10
                    height: parent.height
                }
                Text{
                    width: parent.width*2/5
                    height: parent.height
                    text:"With Second: "
                    color: "white"
                    verticalAlignment: Text.AlignVCenter
                }
                Switch {
                    id:id_isShowSecond
                    width: parent.width*3/8 >100 ? 100 : parent.width*3/8
                    height: parent.height/2
                    anchors.verticalCenter: parent.verticalCenter

                    style: SwitchStyle {
                        groove: Rectangle {
                            implicitWidth: control.width
                            implicitHeight: control.height
                            radius: control.height/2
                            color: control.checked ? "green" : "gray"
                            Row{
                                anchors.fill: parent
                                Text{
                                    width: control.width/2
                                    height: control.height
                                    text:"ON"
                                    font.pixelSize: height/2
                                    color: "white"
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                }
                                Text{
                                    x:control.width/2
                                    width: control.width/2
                                    height: control.height
                                    text:"OFF"
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
                }
            }
        }
    }

    property int selectType: 0
    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        nameFilters: [ "Image files (*.jpg *.png)", "All files (*)" ]
        //folder: shortcuts.home
        selectMultiple: false
        onAccepted:{
            switch(selectType){
            case 1:
                dResPath = fileUrl
                break;
            case 2:
                loadResource(fileUrl,2);
                break;
            case 3:
                loadResource(fileUrl,3);
                break;
            default:
                return;
            }
        }

        onRejected: {
            pushResource();
        }
    }

    function loadResource(path,index){
        var listFlr;
        if(index == 2){
            dResPath = path;
            listFlr = GUiEditor.scanClockResource(true,path);
            if(listFlr.length > 0){
                dResPath = listFlr.shift();
                panel.slt_dResource(listFlr);
            }
        }else if (index == 3){
            tResPath = path;
            listFlr = GUiEditor.scanClockResource(false,path);
            if(listFlr.length > 0){
                tResPath = listFlr.shift();
                panel.slt_tResource(listFlr);
            }
        }else{

        }
    }

    function selectImg(index){
        selectType = index;
        switch(index){
        case 1:
            fileDialog.selectFolder = false
            fileDialog.nameFilters =  [ "Image files (*.jpg *.png)", "All files (*)"]
            break;
        case 2:
        case 3:
            fileDialog.selectFolder = true
            fileDialog.nameFilters =  []
            break;
        default:
            return;
        }
        fileDialog.open()
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
        var resList = GUiEditor.scanClockResource(true);//date
        //console.log(resList)
        if(resList.length > 0){
            dResPath = resList.shift();
            panel.slt_dResource(resList);
        }

        resList = GUiEditor.scanClockResource(false);//time
        if(resList.length > 0){
            tResPath = resList.shift();
            panel.slt_tResource(resList);
        }
        var proList = GUiEditor.pullProperty(elementName);
        paraProperty(proList);
    }

    function pushResource(){
        var list;
        if(panel.hasOwnProperty("slt_dgetRes")){
            list = panel.slt_dgetRes();
            GUiEditor.createDateArea(slt_dgetRect(),list,m_DateStyle)
        }

        if(panel.hasOwnProperty("slt_tgetRes")){
            list = panel.slt_tgetRes();
            GUiEditor.createTimeArea(slt_tgetRect(),list,m_ShowSecond)
        }
    }

    function slt_tRect(rt){
        m_tx = rt.x;
        m_ty = rt.y;
        m_th = rt.width;
        m_tw = rt.height;
    }

    function slt_dRect(rt){
        m_dx = rt.x;
        m_dy = rt.y;
        m_dh = rt.width;
        m_dw = rt.height;
    }

    function paraProperty(list){
        var i;
        var pair;
        for(i=0; i<list.length;i++){
            pair = list[i].split(":")
            if(pair.length != 2)
                continue;

            switch(pair[0]){
            case "Format":
                m_DateStyle = pair[1];
                break;
            case "dRect":
                panel.slt_dsetRect(pair[1])
                break;
            case "WithSecond":
                if(pair[1] =="Yes")
                    m_ShowSecond = true
                else
                    m_ShowSecond = false
                break;
            case "tRect":
                panel.slt_tsetRect(pair[1])
                break;
            default:
                break;
            }
        }
    }
}
