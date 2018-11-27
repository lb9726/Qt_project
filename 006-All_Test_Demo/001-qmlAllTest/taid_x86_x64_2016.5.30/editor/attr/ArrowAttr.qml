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
    property bool oriention:true
    property int arrortype: 0
    signal sig_attr_resize(int x,int y,int w,int h)
    property string m_background: ""

    property alias uparror : id_uparror.source
    property alias downarror : id_downarror.source
    property alias doublearror : id_doublearror.source
    property alias defaultArrow : id_defaultArrow.source

    property int speedLeve: 1

    onOrientionChanged: {
        //if(panel.previewModel){
        if(oriention){
            m_background = uparror
        }else{
            m_background = downarror
        }
        //}
    }

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
                        font.family:"微软雅黑"
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
                //id:testpanel
                font.family:"微软雅黑"
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
                Text{
                    width: parent.width*2/5
                    height: parent.height
                    text:"Up"
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                Rectangle{
                    width: height
                    height: parent.height
                    color: "transparent"
                    border.width: 1 //id_uparror.source == "" ? 1 : 0
                    border.color: "white"
                    Image {
                        id: id_uparror
                        anchors.fill: parent
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            getArror(1);
                        }
                        z:-10
                    }
                }
            }
            Row{
                width: root.width
                height:root.height/10
                Text{
                    font.family:"微软雅黑"
                    width: parent.width*2/5
                    height: parent.height
                    text:"Down"
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                Rectangle{
                    width: height
                    height: parent.height
                    color: "transparent"
                    border.width: 1 //id_downarror.source == "" ? 1 : 0
                    border.color: "white"
                    Image {
                        id: id_downarror
                        anchors.fill: parent
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            getArror(2);
                        }
                        z:-10
                    }
                }
            }

            Row{
                visible: false
                width: root.width
                height:root.height/10
                Text{
                    width: parent.width*2/5
                    height: parent.height
                    text:"Double"
                    color: "white"
                    font.family:"微软雅黑"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                Rectangle{
                    width: height
                    height: parent.height
                    color: "transparent"
                    border.width: 1 //id_doublearror.source == "" ? 1 : 0
                    border.color: "white"
                    Image {
                        id: id_doublearror
                        anchors.fill: parent
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            getArror(3);
                        }
                        z:-10
                    }
                }
            }
            Row{
                visible: false
                width: root.width
                height:root.height/10
                Text{
                    font.family:"微软雅黑"
                    width: parent.width*2/5
                    height: parent.height
                    text:"Default"
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                Rectangle{
                    width: height
                    height: parent.height
                    color: "transparent"
                    border.width: 1 //id_defaultArrow.source == "" ? 1 : 0
                    border.color: "white"
                    Image {
                        id: id_defaultArrow
                        anchors.fill: parent
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            getArror(100);
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
                //id:testpanel
                font.family:"微软雅黑"
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
                height:root.height/10 > 50 ? 50 :root.height/10
                anchors.horizontalCenter: parent.horizontalCenter
                ExclusiveGroup {
                    id: tabPositionGroup

                }

                Item{
                    width: parent.width/8
                    height: parent.height
                }

                RadioButton {
                    id:id_selectSlow
                    text: "Slow"
                    checked: true
                    width:parent.width/4
                    exclusiveGroup: tabPositionGroup
                    style: RadioButtonStyle {
                        indicator: Rectangle {
                            implicitWidth: 16
                            implicitHeight: 16
                            radius: 9
                            //border.color: control.activeFocus ? "darkblue" : "gray"
                            border.width: 1
                            Rectangle {
                                anchors.fill: parent
                                visible: control.checked
                                color: "#555"
                                radius: 9
                                anchors.margins: 4
                            }
                        }
                        label:Text{
                            color: "white"
                            text:control.text
                        }
                        background: Item{}
                    }
                    onCheckedChanged: {
                        if(checked)
                            speedLeve = 1
                    }
                }

                Item{
                    width: parent.width/4
                    height: parent.height
                }

                RadioButton{
                    id:id_selectMiddle
                    text: "Middle"
                    width:parent.width/4
                    exclusiveGroup: tabPositionGroup
                    style: RadioButtonStyle {
                        indicator: Rectangle {
                            implicitWidth: 16
                            implicitHeight: 16
                            radius: 9
                            border.color: control.activeFocus ? "darkblue" : "gray"
                            border.width: 1
                            Rectangle {
                                anchors.fill: parent
                                visible: control.checked
                                color: "#555"
                                radius: 9
                                anchors.margins: 4
                            }
                        }
                        label:Text{
                            color: "white"
                            text:control.text
                        }
                        //background: Rectangle{
                        //}
                    }
                    onCheckedChanged: {
                        if(checked)
                            speedLeve = 2
                    }
                }
            }
            Row{
                width: root.width
                height:root.height/10 > 50 ? 50 :root.height/10
                anchors.horizontalCenter: parent.horizontalCenter
                Item{
                    width: parent.width/8
                    height: parent.height
                }
                RadioButton {
                    id:id_selectFast
                    text: "Fast"
                    width:parent.width/4
                    exclusiveGroup: tabPositionGroup
                    style: RadioButtonStyle {
                        indicator: Rectangle {
                            implicitWidth: 16
                            implicitHeight: 16
                            radius: 9
                            border.color: control.activeFocus ? "darkblue" : "gray"
                            border.width: 1
                            Rectangle {
                                anchors.fill: parent
                                visible: control.checked
                                color: "#555"
                                radius: 9
                                anchors.margins: 4
                            }
                        }
                        label:Text{
                            color: "white"
                            text:control.text
                        }
                        background: Item{}
                    }
                    onCheckedChanged: {
                        if(checked)
                            speedLeve = 3
                    }
                }
                Item{
                    width: parent.width/4
                    height: parent.height
                }
                RadioButton {
                    id:id_selectHightest
                    text: "Hightest"
                    width:parent.width/4
                    exclusiveGroup: tabPositionGroup
                    style: RadioButtonStyle {
                        indicator: Rectangle {
                            implicitWidth: 16
                            implicitHeight: 16
                            radius: 9
                            border.color: control.activeFocus ? "darkblue" : "gray"
                            border.width: 1
                            Rectangle {
                                anchors.fill: parent
                                visible: control.checked
                                color: "#555"
                                radius: 9
                                anchors.margins: 4
                            }
                        }
                        label:Text{
                            color: "white"
                            text:control.text
                        }
                        background: Item{}
                    }
                    onCheckedChanged: {
                        if(checked)
                            speedLeve = 4
                    }
                }
            }
        }
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        nameFilters: [ "Image files (*.jpg *.png)", "All files (*)" ]
        //folder: shortcuts.home
        onAccepted: {
            //GUiEditor.setComRc(elementName,gLayoutOriental,arrortype,fileUrl);
            setImg(arrortype,fileUrl);
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
    function getArror(index){
        arrortype = index;
        fileDialog.open();
    }
    function setImg(index,path){
        switch(index){
        case 1:
            uparror = path;
            m_background = path;
            break;
        case 2:
            downarror = path;
            m_background = path;
            break;
        case 3:
            doublearror = path;
            break;
        case 100:
            defaultArrow = path;
            break;
        default:
            return;
        }
    }

    Timer{
        interval: 1000; running: true; repeat: false
        //onTriggered: pullResource()
    }

    function pullResource(){
        var res = GUiEditor.pullResource(elementName);
        uparror = res[0] == "" ?res[0]: rootWindow.filePrefix + res[0];
        downarror = res[1] == "" ?res[1]: rootWindow.filePrefix + res[1];
        if(uparror != ""){
            m_background = uparror;
        }else if(downarror != ""){
            m_background = downarror;
        }else{

        }
        var proList = GUiEditor.pullProperty(elementName);
        paraProperty(proList);
    }

    function pushResource(){
        var list = new Array;
        list.push(uparror);
        list.push(downarror);
        GUiEditor.pushResource(elementName,list);
        var prolist = new Array;
        list.push("Speed:"+getSpeed());
        GUiEditor.pushArrowProperty(prolist);
    }

    function paraProperty(list){
        var i;
        var pair;
        for(i=0; i<list.length;i++){
            pair = list[i].split(":")
            if(pair.length != 2)
                continue;

            switch(pair[0]){
            case "Speed":
                setSpeed(pair[1]);
                break;
            default:
                break;
            }
        }
    }

    function getSpeed(){
        if(id_selectSlow.checked)
            return "Slow"
        else if (id_selectMiddle.checked){
            return "Middle"
        }else if (id_selectFast.checked){
            return "Fast"
        }else if (id_selectHightest.checked){
            return "Hightest"
        }else{
            return "Middle"
        }
    }

    function setSpeed(speed){
        switch(speed){
        case "Slow":
            speedLeve = 1
            id_selectSlow.checked = true
            break;
        case "Middle":
            speedLeve = 2
            id_selectMiddle.checked = true
            break;
        case "Fast":
            speedLeve = 3
            id_selectFast.checked = true
            break;
        case "Hightest":
            speedLeve = 4
            id_selectHightest.checked = true
            break;
        default:
            speedLeve = 2
            id_selectMiddle.checked = true
            break;
        }
    }
}
