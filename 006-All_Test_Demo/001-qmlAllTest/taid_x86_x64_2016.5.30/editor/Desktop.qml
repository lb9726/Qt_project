import QtQuick 2.5
import "editor.js" as Editor

Rectangle {
    id: id_desktop
    property string elementType: "Desktop"
    property bool oriention: false
    property bool previewModel: gAutoMode
    property bool selecteFlag: false
    property alias elementFocus: scope.focus
    property int panelIndex: 0
    property var panelAttr: null
    property string coverImag: oriention ?panelAttr.m_backgroundV : panelAttr.m_backgroundH
    signal sig_resizing(int re_width, int re_height);

    color: coverImag == "" ? "white" : "transparent"
    //opacity: 0.6
    z: selecteFlag || elementFocus ? 10 : 0
    Text {
        id: elementText
        text: elementType
        font.family: "微软雅黑"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        anchors.centerIn: parent
        visible: coverImag == "" ? true : false
    }

    Image {
        anchors.fill: parent
        source: coverImag == "" ? "" : coverImag
    }

    border.color: selecteFlag || elementFocus? "red": "white"
    border.width: (selecteFlag || elementFocus)&&!previewModel ? 2: 1

    onSelecteFlagChanged: {
        if(selecteFlag)
        {
            scope.forceActiveFocus();
            panelAttr.visible = true
        }
        else
        {
            panelAttr.visible = false
        }
    }

    FocusScope {
        id: scope
        width: childrenRect.width; height: childrenRect.height
    }

    MouseArea {
        id: dragArea
        anchors.fill: parent
        drag.target: parent
        onPressed: {
            scope.forceActiveFocus();
            id_desktop.selecteFlag = true
            Editor.selectAction(elementType, panelIndex);
        }
        z: -10
    }

    onWidthChanged: {
        sig_resizing(width, height);
    }
    onHeightChanged: {
        sig_resizing(width, height);
    }

    Component.onCompleted: {
        initAttrPanel();
    }

    function initAttrPanel()
    {
        var componentattr = Qt.createComponent("attr/DesktopAttr.qml");
        if (componentattr.status === Component.Ready)
        {
            panelAttr = componentattr.createObject(id_attrArea);
            if(panelAttr !== null)
            {
                id_desktop.sig_resizing.connect(panelAttr.setsize);
                panelAttr.elementName = elementType;
                scope.forceActiveFocus();
                id_desktop.selecteFlag = true
                panelAttr.setsize(width, height)
            }
        }
    }
}
