import QtQuick 2.5
import "../editor.js" as Editor

Element{
    id:root
    property bool l_scroll: panelAttr.m_scroll

    onL_scrollChanged: {
        animationCtrl();
    }

    function animationCtrl(){
        if(l_scroll){
            if(textWidth > root.width)
            {
                if(animText.running)
                    animText.restart();
                else
                    animText.start();
            }else{
                animText.stop();
            }
        }else{
            animText.stop();
        }
    }

    property int textWidth:contentTextId.width

    NumberAnimation {
        id:animText
        target: contentTextId
        property: "x"
        duration: 2000
        from: 0
        to:-(textWidth - root.width)
        loops: Animation.Infinite
        alwaysRunToEnd: true
    }

    Item{
        anchors.fill: parent
        clip: true
        Text{
            id:contentTextId
            anchors.verticalCenter: parent.verticalCenter
            text:panelAttr.showText
            wrapMode:Text.Wrap
            font:panelAttr.textFont
            color: panelAttr.textColor
            onTextChanged: {
                animationCtrl();
            }
        }
    }

    Rectangle{
        visible: panelAttr.showText == "" ? true : false
        anchors.fill: parent
        z:-10
    }
}
