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


    property font textFont: Qt.font({ family: "Arial", pixelSize: 20})
    property color textColor: Qt.rgba(255,255,255,255)

    property string showText: ""    
    property bool m_ShowSecond:false //id_scroll.checked
    property string m_Split:":"

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

            Row{
                //visible: false
                width: root.width
                height:40
                Item{
                    width: parent.width/10
                    height: parent.height
                }

                Text{
                    width: parent.width*2/5
                    height: parent.height
                    text:"Color :"
                    font.family:"微软雅黑"
                    color: "white"
                    //horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                Rectangle{
                    width: parent.width*1/5 -10
                    height: 30
                    anchors.verticalCenter: parent.verticalCenter
                    color: textColor
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            colorDialog.open()
                        }
                    }
                }
            }
            Row{
                //visible: false
                width: root.width
                height:40
                Item{
                    width: parent.width/10
                    height: parent.height
                }
                Text{
                    width: parent.width*2/5
                    height: parent.height
                    text:"Font :"
                    color: "white"
                    font.family:"微软雅黑"
                    //horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                Rectangle{
                    width: parent.width*2/5 -10
                    height: 30
                    anchors.verticalCenter: parent.verticalCenter
                    clip: true
                    //color: "transparent"
                    border.width: 2
                    Text{
                        id:id_fontName
                        text:"Arial"
                        font.family:"微软雅黑"
                        anchors.centerIn: parent
                        color: "black"
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            fontDialog.open()
                        }
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
                    height: parent.height
                    text:"Show Seconds :"
                    font.family:"微软雅黑"
                    color: "white"
                    //horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                Switch {
                    id:id_scroll
                    width: parent.width*3/8 >100 ? 100 : parent.width*3/8
                    height: parent.height/2

                    onCheckedChanged: {
                        if(m_ShowSecond != checked)
                            m_ShowSecond = checked;
                    }

                    anchors.verticalCenter: parent.verticalCenter

                }
            }
        }
    }

    ColorDialog {
        id: colorDialog
        title: "Please choose a color"
        onAccepted: {
            textColor = colorDialog.color
        }
    }

    FontDialog {
        id: fontDialog
        title: "Please choose a font"
        font: textFont
        onAccepted: {
            id_fontName.text = font.family +","+font.pixelSize;
            textFont = font;
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
        var proList = GUiEditor.pullProperty(elementName);
        paraProperty(proList);
    }
    function pushResource(){
        var list = new Array;
        list.push("Font@"+textFont.family);
        list.push("PixelSize@"+textFont.pixelSize);
        list.push("Bold@"+textFont.bold ? "true":"false");
        list.push("Color@"+textColor);
        list.push("Split@"+m_Split);
        list.push("Format@"+m_ShowSecond ? "hh:mm:ss":"hh:mm");

        GUiEditor.pushTimeProperty(list);
    }
    function paraProperty(list){
        var i;
        var pair;
        //console.log(list);
        for(i=0; i<list.length;i++){
            pair = list[i].split("@")
//            if(pair[0] == "Format"){
//                if(pair.length == 3){
//                    id_scroll.checked = false
//                    m_ShowSecond = false
//                }else if(pair.length == 4){
//                    id_scroll.checked = true
//                    m_ShowSecond = true
//                }else{
//                    id_scroll.checked = false
//                    m_ShowSecond = false
//                }
//                continue;
//            }

            if(pair.length != 2)
                continue;
            switch(pair[0]){
            case "Font":
                textFont.family = pair[1];
                break;
            case "PixelSize":
                textFont.pixelSize = parseInt(pair[1]);
                break;
            case "Color":
                textColor = pair[1];
                break;
            case "Bold":
                if(pair[1] == "true")
                    textFont.bold = true;
                else
                    textFont.bold = false;
                break;
            case "Split":
                if(pair[1] != ""){
                    m_Split = pair[1]
                }
                break;
            case "Format":
                //console.log(pair[1])
                if(pair[1] == "hh:mm:ss"){
                    //console.log("hh:mm:ss")
                    id_scroll.checked = true
                    m_ShowSecond = true
                }else{
                    //console.log("hh:mm")
                    id_scroll.checked = false
                    m_ShowSecond = false
                }
                    //id_scroll.checked  = pair[1];
                break;
            default:
                break;
            }
        }
        id_fontName.text = textFont.family +","+textFont.pixelSize;
    }
}
