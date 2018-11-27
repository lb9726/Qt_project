import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.2
import QtMultimedia 5.5
import "../editor"
import "../editor/editor.js" as Editor
Rectangle {
    id: id_designItem
    color: mainColor
    property size hResolution: Qt.size(200,200)
    property size vResolution: Qt.size(200,200)
    property bool hvStatus: false //false-horizontal;true-vertical
    property bool l_AutoMode: id_title.m_AutoMode
    SplitView {
        anchors.fill: parent
        orientation: Qt.Vertical
        Header {
            id: id_title
            width: parent.width
            height: parent.height/14
            Layout.minimumHeight: 60
            Layout.maximumHeight: 80
            reslutionInfo: dropArea.width+" x "+dropArea.height
            onScreenShot: {
                dropArea.grabToImage(function(result) {
                    result.saveToFile(rootWindow.projectPath+"grabImage.png");
                });
            }

            onSaveProject: {
                showBusy();
                publishCtrl();
                GUiEditor.savePara();
                GUiEditor.orderResouce();
                hideBusy();
            }
        }
        SplitView {
            width: parent.width
            height: parent.height - id_title.height
            orientation: Qt.Horizontal
            anchors.bottom: parent.bottom
            //Row {
            //id: id_opArea
            //width: parent.width
            //height: parent.height - id_title.height
            //anchors.bottom: parent.bottom
            //anchors.fill: parent
            CtrlList {
                id: id_toolbox
                width: parent.width*2/18
                height: parent.height
                //Layout.minimumWidth: 60
                //Layout.maximumWidth: 120
            }
            //                Rectangle {
            //                    width: 2
            //                    height: parent.height
            //                    opacity: 0.5
            //                }
            Rectangle {
                id: id_dropParent

                width: parent.width*12/18
                height: parent.height

                ScrollView {
                    id: id_dropParentScroll
                    width: parent.width
                    height: parent.height
                    style: ScrollViewStyle {
                        //transientScrollBars: true
                        handle: Item {
                            implicitWidth: 15
                            implicitHeight: 15
                            Rectangle {
                                color: "grey"
                                anchors.fill: parent
                            }
                        }
                        scrollBarBackground: Rectangle {
                            implicitWidth: 15
                            implicitHeight: 15
                            color: mainColor
                        }
                    }

                    Item {
                        id: dropArea
                        width: hvStatus ? vResolution.width: hResolution.width
                        height: hvStatus ? vResolution.height: hResolution.height
                        anchors.centerIn: parent
                        //enabled: false
                        z: 10
                        Desktop {
                            id: id_UIDesktop
                            z: -10
                            anchors.fill: parent
                        }

                    }
                }
            }

            //                Rectangle {
            //                    width: 2
            //                    height: parent.height
            //                    opacity: 0.5
            //                }

            Attribute {
                id: id_attrArea
                width:  parent.width*4/18
                height: parent.height
                //Layout.minimumWidth: 100
                //Layout.maximumWidth: 180
            }
        }
    }
    Connections {
        target: id_title
        onSwitchHV: {
            GUiEditor.setCurDirection(hv);
            gLayoutOriental = hvStatus = hv;
            for(var item in dropArea.children)
            {
                var elementItem = dropArea.children[item];
                if(!elementItem)
                {
                    continue;
                }
                if(elementItem.hasOwnProperty("oriention"))
                {
                    elementItem.oriention = hv;
                }
            }
        }
    }

    Connections {
        target: rootWindow
        onDyCreate: {
            Editor.directCreatePanel(panelName, rectH, rectV);
        }
    }

    onL_AutoModeChanged: {
        gAutoMode = l_AutoMode;
        if(l_AutoMode)
        {
            id_toolbox.state = "Auto";
            id_attrArea.state = "Auto";
            id_UIDesktop.enabled = false;
            startAutoTest();
        }
        else
        {
            id_toolbox.state = "Normal";
            id_attrArea.state = "Normal";
            id_UIDesktop.enabled = true;
            endAutoTest();
        }
    }

    function initReslution()
    {
        hResolution = GUiEditor.getReslution(false)
        vResolution = GUiEditor.getReslution(true)
    }

    function releaseCtrl()
    {
        for(var item in dropArea.children)
        {
            var elementItem = dropArea.children[item];
            if(!elementItem)
            {
                continue;
            }

            if(elementItem.hasOwnProperty("destroy"))
            {
                elementItem.destroy();
            }
        }
    }

    Component.onCompleted: {
        //visible = false;
        showBusy();
        initReslution();
    }

    function pullAllResource()
    {
        for(var item in id_attrArea.children)
        {
            var elementItem = id_attrArea.children[item];
            if(!elementItem)
            {
                continue;
            }
            if(elementItem.hasOwnProperty("pullResource"))
            {
                elementItem.pullResource();
            }
        }
    }

    Timer {
        interval: 2000; running: true; repeat: false
        onTriggered: {
            //rootWindow.showBusy();
            GUiEditor.loadUICom();
        }
    }

    Component.onDestruction: {
        releaseCtrl();
    }

    function publishCtrl()
    {
        for(var item in id_attrArea.children)
        {
            var elementItem = id_attrArea.children[item];
            if(!elementItem)
            {
                continue;
            }
            if(elementItem.hasOwnProperty("pushResource"))
            {
                elementItem.pushResource();
            }
        }
    }

    /* Auto test function */
    property int countBase: 1
    property int starFlr: 1
    property int endFlr: 20
    property int autoCurFlr: 1
    property bool autoDirection: true // true-上行，false-下行

    property var eArrow: null
    property var eFloor: null
    property var eFunction: null
    property var eText: null
    property var eMedia: null
    property var ePicture: null
    property var eVideo: null

    onAutoCurFlrChanged: {
        //楼层
        if(eFloor && eFloor.hasOwnProperty("updateFlr"))
        {
            eFloor.updateFlr(autoCurFlr)
            playMusic(autoCurFlr);
        }
    }

    onAutoDirectionChanged: {
        //箭头
        if(eArrow && eArrow.hasOwnProperty("oriention"))
        {
            eArrow.oriention = autoDirection;
        }
    }

    Timer {
        id: auotTestTimer
        interval: 2000
        repeat: true
        onTriggered: {
            if(autoDirection)
            {
                if(autoCurFlr + 1 > endFlr)
                {
                    autoDirection = false
                }
                else
                {
                    autoCurFlr = autoCurFlr +1
                }
            }
            else
            {
                if(autoCurFlr - 1 < starFlr)
                {
                    autoDirection = true
                }
                else
                {
                    autoCurFlr = autoCurFlr -1
                }
            }
        }
    }

    function startAutoTest()
    {
        eArrow = getAttr("Arrow");
        eFloor = getAttr("Floor");
        eFunction = getAttr("Function");
        eText = getAttr("Text");
        ePicture = getAttr("Picture");


        eMedia = getAttr("Media");
        eVideo = getElement("Video");

        if(eText)
        {
            eText.m_scroll = true;
        }

        auotTestTimer.start();
    }

    function endAutoTest()
    {
        auotTestTimer.stop();
        eArrow = null;
        eFloor = null;
        eFunction = null;
        eMedia = null;

        if(eText)
        {
            eText.m_scroll = false;
        }

        if(ePicture)
        {
            ePicture  = null;
        }
        if(eVideo)
        {
            eVideo  = null;
        }
    }

    function getElement(eleName)
    {
        for(var item in dropArea.children)
        {
            var elementItem = dropArea.children[item];
            if(elementItem &&elementItem.hasOwnProperty("elementType"))
            {
                if(elementItem.elementType === eleName)
                {
                    return elementItem;
                }
            }
        }
        return null;
    }

    function getAttr(eleName)
    {
        for(var item in id_attrArea.children)
        {
            var elementItem = id_attrArea.children[item];
            if(elementItem &&elementItem.hasOwnProperty("elementName"))
            {
                if(elementItem.elementName === eleName)
                {
                    return elementItem;
                }
            }
        }
        return null;
    }

    function playMusic(index)
    {
        if(eMedia && eMedia.hasOwnProperty("selectMusic"))
        {
            eMedia.selectMusic(index)
        }
    }
}
