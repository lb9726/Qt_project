import QtQuick 2.7
import QtQuick.Layouts 1.0
import QtQuick.Controls.Styles 1.3
import QtQuick.Controls 1.3

Item {
    id: area4_comp
    property int area1_width: 480
    property int area4_height: 140
    property variant prestrList: [4] ;
    property variant curstrList: [4] ;
    property variant idstrList: [4] ;
    signal languagechanges()
    width: area1_width
    height: area4_height

    Text {
        id: changeLanguage
        x: 0
        y: 0
        text: qsTr("语言切换")
        font.pixelSize: 15
    }

    RadioButton {
        id: language_chinese
        ExclusiveGroup { id: langGroup }
        exclusiveGroup: langGroup
        x: 64
        y: 31
        text: qsTr("中文")
        onCheckedChanged: {
            if (checked)
            {
                elcusiveDeal(true, false, false, false);
                Ctranslator.load(0);
            }
        }
    }

    RadioButton {
        id: language_english
        exclusiveGroup: langGroup
        x: 163
        y: 31
        text: qsTr("English")
        onCheckedChanged: {
            if (checked)
            {
                elcusiveDeal(false, false, true, false);
                Ctranslator.load(2);
            }
        }
    }

    RadioButton {
        id: language_tradition
        exclusiveGroup: langGroup
        x: 64
        y: 73
        text: qsTr("繁體")
        onCheckedChanged: {
            if (checked)
            {
                elcusiveDeal(false, true, false, false);
                Ctranslator.load(1);
            }
        }
    }

    RadioButton {
        id: language_russian
        exclusiveGroup: langGroup
        x: 163
        y: 73
        width: 89
        height: 16
        text: qsTr("русский язык")
        onCheckedChanged: {
            if (checked)
            {
                MediaScreen.setIndex(3);
                id_mediaScreenHelper.openChangeLayout();
            }
        }
    }

    function getLastChecked()
    {
        console.log("getLastChecked")
        for (var i = 0; i < 4; i++)
        {
            if (i !== 3 && prestrList[i] === true)
            {
                console.log("prestrList checked i = " + i + " is true")
                return i;
            }
        }
        return 3;
    }

    function ischeckedchange()
    {
        idstrList[0] = language_chinese;
        idstrList[1] = language_tradition;
        idstrList[2] = language_english;
        idstrList[3] = language_russian;

        curstrList[0] = language_chinese.checked;
        curstrList[1] = language_tradition.checked;
        curstrList[2] = language_english.checked;
        curstrList[3] = language_russian.checked;

        for (var i = 0; i < 4; ++i)
        {
            if (i !== 3 && prestrList[i] === true)
            {
                console.log("prestrList checked i = " + i + " is true")
                idstrList[i].checked = true;
                MediaScreen.setIndex(i); // 重新将index值设置会原来的值
                idstrList[3].checked = false;
            }
            console.log("curstrList" + i + " =", curstrList[i] )
        }

        for (var j = 0; j < 4; j++)
        {
            if (prestrList[j] === true)
            {
                console.log("j = "+ j + " is true")
            }
            console.log("curstrList" + j + " =", curstrList[j] )
            console.log("prestrList" + j + " =", prestrList[j] )
        }
        recordchecked(language_chinese, language_tradition, language_english, language_russian)
    }

    function recordchecked(checked0, checked1, checked2, checked3)
    {
        prestrList[0] = checked0.checked;
        prestrList[1] = checked1.checked;
        prestrList[2] = checked2.checked;
        prestrList[3] = checked3.checked;
        for (var j = 0; j < 4; j++)
        {
            console.log("prestrList" + j + " =", prestrList[j] )
        }
    }

    function elcusiveDeal(bool0, bool1, bool2, bool3)
    {
        prestrList[0] = bool0;
        prestrList[1] = bool1;
        prestrList[2] = bool2;
        prestrList[3] = bool3;
        for (var j = 0; j < 4; j++)
        {
            console.log("prestrList_new" + j + " =", prestrList[j] )
        }
    }

    function are4_language()
    {
        console.log("语言改变 Area4")
        changeLanguage.text = qsTr("语言切换")
//        language_chinese.text = qsTr("中文")
//        language_tradition.text = qsTr("繁体")
//        language_english.text = qsTr("英语")
//        language_russian.text = qsTr("俄语")
    }

    function buttonChecked()
    {
        if (3 !== MediaScreen.getPreIndex())// 区别第一次和俄语界面切换回来的情况
        {
            console.log("first enter main.qml last is not in russian");
            MediaScreen.setIndex(MediaScreen.getSystemLanguage());
        }

        var res = MediaScreen.getIndex()
        console.log("buttonChecked function res = ", res);
        if (-1 === res || 0 === res)
        {
            language_chinese.checked = true;
        }
        else if (1 === res)
        {
            language_tradition.checked = true;
        }

        else if (2 === res)
        {
            language_english.checked = true;
        }

    }

    Connections {
        target: Ctranslator
        onLanguageChanges: {
            are4_language();
        }
    }
    Component.onCompleted: {
        buttonChecked();
        recordchecked(language_chinese, language_tradition,
                      language_english, language_russian)

    }
}
