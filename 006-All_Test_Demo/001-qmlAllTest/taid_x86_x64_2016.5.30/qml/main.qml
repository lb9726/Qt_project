import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.2
import QtQml.Models 2.1
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.0
import "../editor/editor.js" as Editor
ApplicationWindow {
    id: rootWindow
    visible: true
    property alias windowWidth: rootWindow.width //20 //Screen.width/2;
    property alias windowHeight: rootWindow.height //20 //Screen.height/2;
    property color mainColor: "#3D4488"
    property color viceColor: "#0C1029"
    property real themeOpacity: 0.6
    property string filePrefix: "file: ///"

    property bool projectStatus: false;
    property string deviceModel: "";
    property string projectPath: "";

    onProjectStatusChanged: {
        if(projectStatus) {
            deviceModel = GUiEditor.getDeviceModel();
            projectPath = GUiEditor.getProjectDir()
        }
    }

    //flags: Qt.CustomizeWindowHint|Qt.WindowMinimizeButtonHint & Qt.WindowMaximizeButtonHint
    minimumHeight: 480
    minimumWidth: 800
    //onWidthChanged: {windowWidth = width}
    //onHeightChanged: {windowHeight = height}
    Behavior on width {NumberAnimation { duration: 300 }}
    Behavior on height {NumberAnimation { duration: 300 }}
    title: "taid v1.2.0"
    Rectangle {
        width: windowWidth
        height: windowHeight
        color: mainColor
        Loader {
            id: centerWindows
            anchors.fill: parent
            asynchronous: true
            focus: true
        }
    }

    StackView {
        id: stack
        anchors.fill: parent
        focus: true
        initialItem: HomePanel {}
        enabled: !id_busyIndicator.running
        opacity: enabled ? 1 : themeOpacity
    }

    BusyIndicator {
        id: id_busyIndicator
        running: false
        anchors.centerIn: parent
    }

    function showBusy() {
        if(!id_busyIndicator.running) {
            id_busyIndicator.running = true
        }
    }

    function hideBusy() {
        id_busyIndicator.running = false
    }

    function openProject() {
        stack.pop();
    }

    property Item r_homepanel: null
    property Item r_multMedia: null
    property Item r_uieditor: null
    property Item r_updateexe: null
    property Item r_published: null
    property bool gLayoutOriental: false //false horizental , true verical
    property bool gAutoMode: false

    onGLayoutOrientalChanged: {
        GUiEditor.setCurDirection(gLayoutOriental);
    }

    Component.onCompleted: {
        //GUiEditor.setCurDirection(gLayoutOriental);
    }

    Component.onDestruction: {
        if(r_homepanel) {
            r_homepanel.destroy();
        }
        if(r_multMedia) {
            r_multMedia.destroy()
        }
        if(r_uieditor) {
            r_uieditor.destroy()
        }
        if(r_updateexe) {
            r_updateexe.destroy()
        }
        if(r_published) {
            r_published.destroy()
        }
    }


    function uiLogicalEngine(index,subIndex) {
        switch(index) {
        case 1: //首界面
            switch(subIndex) {
            case 0:
                quitTaid();
                break;
            case 1: //在线制作
                loadPanel(8);
                break;
            case 2: //本地制作
                loadPanel(6);
                break;
            default:
                break;
            }
            break;
        case 2: //登录
            switch(subIndex) {
            case 0:
                loadPanel(0);
                break;
            case 1:
                stack.pop();
                break;
            case 2:
                break;
            default:
                break;
            }
            break;
        case 3: //设备选择
            switch(subIndex) {
            case 0:
                loadPanel(7);
                break;
            case 1:
                stack.pop();
                break;
            default:
                break;
            }
            break;
        case 4: //在线获取UI
            switch(subIndex) {
            case 0:
                stack.pop(null);
                loadPanel(2);
                break;
            case 1:
                stack.pop();
                break;
            case 2:
                break;
            default:
                break;
            }
            break;
        case 5: //本地获取UI
            switch(subIndex) {
            case 0:
                //stack.pop(null);
                loadPanel(2);
                break;
            case 1:
                stack.pop();
                break;
            case 2:
                break;
            default:
                break;
            }
            break;
        case 6: //主功能
            //r_homepanel = stack.pop();
            switch(subIndex) {
            case 0:
                //stack.pop();
                stack.pop()
                clearPanel();
                break;
            case 1:
                quitTaid();
                break;
            case 2:
                loadPanel(3);
                break;
            case 3:
                loadPanel(9);
                break;
            case 4:
                loadPanel(1);
                break;
            case 5:
                loadPanel(5);
                break;
            default:
                break;
            }
            break;
        case 7: //多媒体资源更新
            stack.pop();
            switch(subIndex) {
            case 0:
                //loadPanel(2);
                break;
            case 1:
                loadPanel(5);
                break;
            default:
                break;
            }
            break;
        case 8: //界面设计
            stack.pop();
            switch(subIndex) {
            case 0:
                //loadPanel(2);
                break;
            case 1:
                loadPanel(5);
                break;
            default:
                break;
            }
            break;
        case 9: //程序升级
            stack.pop();
            switch(subIndex) {
            case 0:
                //loadPanel(2);
                break;
            case 1:
                loadPanel(5);
                break;
            default:
                break;
            }
            break;
        case 10: //发布
            stack.pop();
            switch(subIndex) {
            case 0:
                //loadPanel(2);
                break;
            default:
                break;
            }
            break;
        default:
            return;
        }
    }

    function quitTaid() {
        GUiEditor.savePara()
        Qt.quit();
    }

    function loadPanel(index) {
        switch(index) {
        case 0:
            stack.push( {item: "qrc: /qml/DeviceSelect.qml"})
            break;
        case 1:
            if(r_updateexe)
                stack.push(r_updateexe)
            else
                r_updateexe = stack.push( {item: "qrc: /qml/ExeUpdate.qml", immediate: true,destroyOnPop: false})
            break;
        case 2:
            if(r_homepanel)
                stack.push(r_homepanel)
            else
                r_homepanel = stack.push( {item: "qrc: /qml/HomePanel.qml", immediate: true,destroyOnPop: false})
            break;
        case 3:
            if(r_multMedia)
                stack.push(r_multMedia)
            else
                r_multMedia = stack.push( {item: "qrc: /qml/MultMedia.qml", immediate: true,destroyOnPop: false})
            break;
        case 4:
            stack.push( {item: "qrc: /qml/OnLine.qml"})
            break;
        case 5:
            if(r_published) {
                stack.push(r_published)
                r_published.refreshDevice();
            }
            else
                r_published =stack.push( {item: "qrc: /qml/Publish.qml", immediate: true,destroyOnPop: false})
            break;
        case 6:
            stack.push( {item: "qrc: /qml/UIFileOff.qml"})
            break;
        case 7:
            stack.push( {item: "qrc: /qml/UIFileOn.qml"})
            break;
        case 8:
            stack.push( {item: "qrc: /qml/LogIn.qml"})
            break;
        case 9:
            if(r_uieditor)
                stack.push(r_uieditor)
            else
                r_uieditor =stack.push( {item: "qrc: /qml/UIDesign.qml", immediate: true,destroyOnPop: false})
            break;
        default:
            return;
        }
    }

    function clearPanel() {
        if(r_homepanel) {
            r_homepanel.destroy();
            r_homepanel =  null
        }

        if(r_multMedia) {
            r_multMedia.destroy();
            r_multMedia =  null
        }

        if(r_uieditor) {
            r_uieditor.destroy();
            r_uieditor =  null
        }

        if(r_updateexe) {
            r_updateexe.destroy();
            r_updateexe =  null
        }

        if(r_published) {
            r_published.destroy();
            r_published =  null
        }
    }

    signal dyCreate(string panelName,rect rectH,rect rectV);

    function dynamicCreate(panelName,rectH,rectV) {
        showBusy();
        if("Finish" == panelName) {
            if(r_uieditor && r_uieditor.hasOwnProperty("pullAllResource")) {
                r_uieditor.pullAllResource();
            }
            hideBusy();
        }else {
            dyCreate(panelName,rectH,rectV);
        }
    }


    signal spublishProgress(int deviceOrder, int proValue);
    signal spublishError(string info);
    signal spublishFinish();

    function publishProgress(deviceOrder,proValue) {
        console.log("#: ",deviceOrder,proValue)
        spublishProgress(deviceOrder,proValue)

    }



    function publishError(info) {
        console.log("#publishError: " + info)
        spublishError(info);

    }

    function publishFinish(info) {
        console.log("#Finish: ")
        spublishFinish()

    }

    signal scompressProgress(int proValue);
    signal scompressfinish();
    function compressProgress(proValue) {
        scompressProgress(proValue)
    }
    function compressfinish(info) {
        scompressfinish();
    }
}
