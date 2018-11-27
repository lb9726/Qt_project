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
    property real m_StyleScale:0.75
    property alias m_ColokStyle:id_Layout.checked  //0-date:up,Arrow:down;1-date:down,Arrow:up;
    property alias m_Direction:id_direction.checked // 0- up/down ; 1-right/left
    property real m_btnSpacing:0.1
    ScrollView {
        anchors.fill: parent
        horizontalScrollBarPolicy:Qt.ScrollBarAlwaysOff
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
                    border.width: id_secondsImg.source == "" ? 1 : 0
                    border.color: "white"
                    Image {
                        anchors.fill: parent
                        source:m_background
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            selectImg(5)
                        }
                        z:-10
                    }
                }
            }

            Item{
                width: root.width
                height: root.height/10 > 30 ? 30:root.height/10
                Row{
                    height: parent.height
                    anchors.centerIn: parent
                    Button{
                        width: height
                        height: parent.height
                        anchors.verticalCenter: parent.verticalCenter
                        style: ButtonStyle {
                            background: Image {
                                source: "qrc:/images/add.png"
                            }
                        }
                        opacity: pressed ? themeOpacity : 1
                        onClicked: {
                            selectImg(1);
                        }
                    }
                    Text{
                        width: parent.width*3/5
                        height: parent.height
                        text:"Date"
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    Button{
                        width: height
                        height: parent.height
                        anchors.verticalCenter: parent.verticalCenter
                        style: ButtonStyle {
                            background: Image {
                                source: "qrc:/images/clear.png"
                            }
                        }
                        opacity: pressed ? themeOpacity : 1
                        onClicked: {
                            dateOrTime = true
                            clearImg();
                        }
                    }
                }
            }
            GridView{
                id:id_DateImgList
                width: root.width
                height:cellHeight/4 * count;

                clip: true
                cellHeight : cellWidth
                cellWidth :  root.width/4 > 60 ? 60 : root.width/4

                delegate:Item{
                    width: id_DateImgList.cellWidth
                    height:id_DateImgList.cellHeight
                    Image{
                        width: parent.width*0.8
                        height:parent.height*0.8
                        anchors.centerIn: parent
                        source: imgPath
                    }
                }
                model:id_DateModel
            }

            Item{
                width: root.width
                height: root.height/10 > 30 ? 30:root.height/10
                Row{
                    height: parent.height
                    anchors.centerIn: parent
                    Button{
                        width: height
                        height: parent.height
                        anchors.verticalCenter: parent.verticalCenter
                        style: ButtonStyle {
                            background: Image {
                                source: "qrc:/images/add.png"
                            }
                        }
                        opacity: pressed ? themeOpacity : 1
                        onClicked: {
                            selectImg(2);
                        }
                    }

                    Text{
                        width: parent.width*3/5
                        height: parent.height
                        text:"Time"
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    Button{
                        width: height
                        height: parent.height
                        anchors.verticalCenter: parent.verticalCenter
                        style: ButtonStyle {
                            background: Image {
                                source: "qrc:/images/clear.png"
                            }
                        }
                        opacity: pressed ? themeOpacity : 1
                        onClicked: {
                            dateOrTime = false
                            clearImg();
                        }
                    }
                }
            }
            GridView{
                id:id_TimeImgList
                width: root.width
                height:cellHeight/4 * count;

                clip: true
                cellHeight : cellWidth
                cellWidth :  root.width/4 > 60 ? 60 : root.width/4

                delegate:Item{
                    width: id_TimeImgList.cellWidth
                    height:id_TimeImgList.cellHeight
                    Image{
                        width: parent.width*0.8
                        height:parent.height*0.8
                        anchors.centerIn: parent
                        source: imgPath
                    }
                }
                model:id_TimeModel
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
                    text:"Dash: "
                    color: "white"
                    //horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                Rectangle{
                    width: height
                    height: parent.height
                    color: "transparent"
                    border.width: id_dashImg.source == "" ? 1 : 0
                    border.color: "white"
                    Image {
                        id: id_dashImg
                        anchors.fill: parent
                    }
                    MouseArea{
                        anchors.fill: parent
                        onDoubleClicked:  {
                            console.log(" selectImg(3)")
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
                    text:"Second: "
                    color: "white"
                    //horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                Rectangle{
                    width: height
                    height: parent.height
                    color: "transparent"
                    border.width: id_secondsImg.source == "" ? 1 : 0
                    border.color: "white"
                    Image {
                        id: id_secondsImg
                        anchors.fill: parent
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            selectImg(4)
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
                    text:"Direction: "
                    color: "white"
                    verticalAlignment: Text.AlignVCenter
                    anchors.verticalCenter: parent.verticalCenter
                }
                Switch {
                    id:id_direction
                    width: parent.width*3/8 >100 ? 100 : parent.width*3/8
                    height: parent.height/2
                    anchors.verticalCenter: parent.verticalCenter
                    style: SwitchStyle {
                        groove: Rectangle {
                            implicitWidth: control.width
                            implicitHeight: control.height
                            radius: control.height/2
                            color: control.checked ? "gray" : "gray"
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
                    text:"Layout: "
                    color: "white"
                    verticalAlignment: Text.AlignVCenter
                    anchors.verticalCenter: parent.verticalCenter
                }
                Switch {
                    id:id_Layout
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
                                    text:"Swap"
                                    font.pixelSize: height/2
                                    color: "white"
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                }
                                Text{
                                    x:control.width/2
                                    width: control.width/2
                                    height: control.height
                                    text:""
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
                height:root.height/10 > 50 ? 50 : root.height/10
                Item{
                    width: parent.width/10
                    height: parent.height
                }

                Text{
                    width: parent.width*2/5
                    height: parent.height
                    text:"Scale: "
                    color: "white"
                    verticalAlignment: Text.AlignVCenter
                }

                Rectangle{
                    width: parent.width/2 >120 ? 120 : parent.width/2
                    height: parent.height/2
                    color: viceColor
                    anchors.verticalCenter: parent.verticalCenter
                    ComboBox {
                        anchors.fill: parent
                        model: [ "3:1","1:1", "1:3",]
                        style: ComboBoxStyle{
                            background:Item{}
                            textColor:"white"
                        }
                        editable:false
                        onCurrentIndexChanged: {
                            switch(currentIndex){
                            case 0:
                                m_StyleScale = 0.75
                                break;
                            case 1:
                                m_StyleScale = 0.5
                                break;
                            case 2:
                                m_StyleScale = 0.25
                                break;
                            default:
                                m_StyleScale = 0.5
                            }
                        }
                    }
                }
            }
            Row{
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
            Row{
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

    ListModel{
        id:id_DateModel
    }

    ListModel{
        id:id_TimeModel
    }

    property int selectType: 0  //1-Date;2-Time;3-Second;4-Dash
    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        nameFilters: [ "Image files (*.jpg *.png)", "All files (*)" ]
        folder: shortcuts.home
        selectMultiple: true
        onAccepted: {
            var i;
            var temp;
            var names;
            var tempName;

            switch(selectType){
            case 1:
                id_DateModel.clear();
                for(i = 0;i<fileUrls.length;i++){
                    temp = fileUrls[i];
                    names = temp.split("/");
                    tempName =  names[names.length - 1];
                    console.log(tempName)
                    id_DateModel.insert(i, {imgPath:temp,fileName:tempName})
                }
                break;
            case 2:
                id_TimeModel.clear();
                for(i = 0;i<fileUrls.length;i++){
                    temp = fileUrls[i];
                    names = temp.split("/");
                    tempName =  names[names.length - 1];
                    console.log(tempName)
                    id_TimeModel.insert(i, {imgPath:temp,fileName:tempName})
                }
                break;
            case 3:
                id_dashImg.source = fileUrl
                break;
            case 4:
                id_secondsImg.source = fileUrl
                break;
            case 5:
                console.log(fileUrl)
                m_background = fileUrl
                break;
            default:
                return;
            }

        }
    }

    function selectImg(index){
        console.log(index)
        selectType = index;
        switch(index){
        case 1:
        case 2:
            fileDialog.selectMultiple = true
            break;
        case 3:
        case 4:
        case 5:
            fileDialog.selectMultiple = false
            break;
        default:
            return;
        }
        fileDialog.open()
    }

    function clearImg(){
        if(selectType ==1)
            id_DateModel.clear();
        else if(selectType ==2)
            id_TimeModel.clear();
        else{
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

    function initLoad(){
        var rclist = GUiEditor.getComRc(elementName)
        for(var i=0;i<rclist.length;i++){
            var temp = rclist[i].split("@");
            m_background = "file://" + temp[1];
        }
    }
}
