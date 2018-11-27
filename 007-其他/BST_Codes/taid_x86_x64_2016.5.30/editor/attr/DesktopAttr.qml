import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.2

Item {
    id:root
    anchors.fill: parent
    property var panel: null
    property string elementName: "Desktop"
    property string m_backgroundH: ""
    property string m_backgroundV: ""

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
            spacing:10
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
                opacity: 0.5
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
                    font.family:"微软雅黑"
                    color: "white"
                    //horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                Item{
                    width: parent.width*2/5 -10
                    height: 30
                    anchors.verticalCenter: parent.verticalCenter
                    Text{
                        id:pro_width
                        anchors.fill: parent
                        color: "white"
                        font.family:"微软雅黑"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
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
                    font.family:"微软雅黑"
                    width: parent.width*2/5
                    height: parent.height
                    text:"height : "
                    color: "white"
                    //horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                Item{
                    width: parent.width*2/5 -10
                    height: 30
                    anchors.verticalCenter: parent.verticalCenter
                    Text{
                        id:pro_height
                        anchors.fill: parent
                        color: "white"
                        font.family:"微软雅黑"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
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
                width: root.width
                height: root.height/10 > 50 ? 50:root.height/10
                text:"Background"
                color: "white"
                font.family:"微软雅黑"
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
                    text:"horizontal: "
                    color: "white"
                    font.family:"微软雅黑"
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
                        source: m_backgroundH
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
                    text:"vertical: "
                    font.family:"微软雅黑"
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
                        source: m_backgroundV
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
        }
    }

    property int selectType: 0
    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        nameFilters: [ "Image files (*.jpg *.png)", "All files (*)" ]
        //folder: shortcuts.home
        selectMultiple: true
        onAccepted: {
            //console.log(fileUrl);
            switch(selectType){
            case 1:
                m_backgroundH = fileUrl
                break;
            case 2:
                m_backgroundV = fileUrl
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
        //console.log(res)
        if(res.length == 1){
            m_backgroundH = res[0] == ""? "" : rootWindow.filePrefix + res[0]
        }
        if(res.length == 2){
            m_backgroundH = res[0] == ""? "" : rootWindow.filePrefix + res[0]
            m_backgroundV = res[1] == ""? "" : rootWindow.filePrefix + res[1]
        }
    }

    function pushResource(){
        var list = new Array;
        list.push(m_backgroundH);
        list.push(m_backgroundV);
        GUiEditor.pushResource(elementName,list);
    }
}
