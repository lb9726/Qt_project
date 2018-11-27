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
    property string m_bottonOnImg :""
    property string m_bottonOffImg:""
    property real m_btnSpacing:0

    property alias m_withUp:id_withUp.checked
    property alias m_withDown:id_withDown.checked

    onM_withDownChanged: {
        if(!m_withUp&&!m_withDown){
            m_withUp = true;
        }
    }
    onM_withUpChanged: {
        if(!m_withUp&&!m_withDown){
            m_withDown = true;
        }
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
                    text:"Released: "
                    color: "white"
                    //horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                Rectangle{
                    width: height
                    height: parent.height
                    color: "transparent"
                    border.width: id_buttonOffImg.source == "" ? 1 : 0
                    border.color: "white"
                    Image {
                        id: id_buttonOffImg
                        anchors.fill: parent
                        source:m_bottonOffImg
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            selectImg(3)
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
                    text:"Pressed: "
                    color: "white"
                    //horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                Rectangle{
                    width: height
                    height: parent.height
                    color: "transparent"
                    border.width: id_buttonOnImg.source == "" ? 1 : 0
                    border.color: "white"
                    Image {
                        id: id_buttonOnImg
                        anchors.fill: parent
                        source: m_bottonOnImg
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
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
                    text:"background: "
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
                        source: m_background
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
                height:root.height/10 > 50 ? 50:root.height/10
                Item{
                    width: parent.width/10
                    height: parent.height
                }

                Text{
                    width: parent.width*2/5
                    height: parent.height/2
                    text:"With Up:"
                    color: "white"
                    //horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                Switch {
                    id:id_withUp
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
                    text:"With Down:"
                    color: "white"
                    //horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                Switch {
                    id:id_withDown
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
                    checked: true
                }
            }
            Row{
                visible: m_withUp && m_withDown
                width: root.width
                height:root.height/10 > 30 ? 30 :root.height/10
                anchors.horizontalCenter: parent.horizontalCenter

                Item{
                    width: parent.width/10
                    height: parent.height
                }

                Text{
                    width: parent.width*2/5
                    height: parent.height/2
                    text:"Spacing:"
                    color: "white"
                    //horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                Rectangle{
                    width: parent.width/4
                    height: parent.height
                    TextInput{
                        id:id_btnSpacing
                        anchors.fill: parent
                        font.pixelSize: height/2
                        color: "black"
                        //text:m_btnSpacing
                        maximumLength: 3
                        verticalAlignment: Text.AlignVCenter
                        onTextChanged: {
                            m_btnSpacing = parseFloat(text)
                            if(m_btnSpacing <0||m_btnSpacing >1){
                                text = ""
                                m_btnSpacing = 0
                            }
                            //console.log(m_btnSpacing)
                        }
                    }
                    clip: true
                }
            }
        }
    }

    property int selectType: 0  //1-Date;2-Time;3-Second;4-Dash
    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        nameFilters: [ "Image files (*.jpg *.png)", "All files (*)" ]
        //folder: shortcuts.home
        selectMultiple: true
        onAccepted: {
            switch(selectType){
            case 1:
                m_background = fileUrl
                break;
            case 2:
                m_bottonOnImg = fileUrl
                break;
            case 3:
                m_bottonOffImg = fileUrl
                break;
            default:
                return;
            }
        }

        onRejected: {
            pushResource()
        }
    }

    function selectImg(index){
        selectType = index;
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
        var res = GUiEditor.pullResource(elementName);
        if(res.length != 5)
            return;
        var tmpstr = res[0];
        m_background = res[0] =="" ? "":rootWindow.filePrefix + res[0];
        tmpstr = parseInt(res[1])
        if(tmpstr == 1){
            m_withUp = true;
            m_withDown = false;
        }else if(tmpstr == 2){
            m_withUp = false;
            m_withDown = true;
        }else if(tmpstr == 3){
            m_withUp = true;
            m_withDown = true;
            m_btnSpacing = parseFloat(res[2]);
            id_btnSpacing.text = m_btnSpacing;
        }else{

        }
        m_bottonOnImg = res[3] =="" ? "":rootWindow.filePrefix + res[3];
        m_bottonOffImg = res[4] =="" ? "":rootWindow.filePrefix + res[4];

    }

    function pushResource(){
        var list = new Array;
        var stat  = 0;
        if(m_withUp && !m_withDown){
            stat = 1;
        }else if(!m_withUp && m_withDown){
            stat = 2;
        }else if(m_withUp && m_withDown){
            stat = 3;
        }

        list.push(m_background);
        list.push(m_bottonOnImg);
        list.push(m_bottonOffImg);
        list.push(stat);
        list.push(m_btnSpacing);
        GUiEditor.pushResource(elementName,list);
    }
}
