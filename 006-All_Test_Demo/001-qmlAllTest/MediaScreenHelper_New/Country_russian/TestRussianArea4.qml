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
        //                text: qsTr("语言切换")
        text: qsTr("Переключатель языка")
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
                //MediaScreen.setIndex(0)
                MediaScreen.setPreIndex(0);
                id_mediaScreenHelper.openChangeLayout()
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
                MediaScreen.setPreIndex(2)
                id_mediaScreenHelper.openChangeLayout()
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
                MediaScreen.setPreIndex(1)
                id_mediaScreenHelper.openChangeLayout()
            }
        }
    }

    RadioButton {
        id: language_russian
        exclusiveGroup: langGroup
        x: 163
        y: 73
        checked: true
        width: 89
        height: 16
        //                    text: qsTr("俄语")
        text: qsTr("русский язык")
        onCheckedChanged: {
            if (checked)
            {
                MediaScreen.setIndex(3);
                id_mediaScreenHelper.openChangeLayout();
            }
        }
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
            if (i !== 3)
            {
                idstrList[i].checked = false;
            }
            else if (i === 3)
            {
                idstrList[3].checked = true;
            }
        }
        recordchecked(language_chinese, language_tradition, language_english, language_russian)
    }

    function recordchecked(checked0, checked1, checked2, checked3)
    {
        prestrList[0] = checked0.checked;
        prestrList[1] = checked1.checked;
        prestrList[2] = checked2.checked;
        prestrList[3] = checked3.checked;
        for (var j = 0; j < 4; ++j)
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
        for (var j = 0; j < 4; ++j)
        {
            console.log("prestrList_new" + j + " =", prestrList[j] )
        }
    }
}

