import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Item {
    property variant prestrList: [4] ;
    property variant curstrList: [4] ;
    property variant idstrList: [4] ;
    ColumnLayout {
        spacing: 20
        RowLayout {
            Label {
                text: qsTr("语言切换")
            }
        }

        RowLayout {
            spacing: 80
            Rectangle {
                width: 20
                color: "transparent"
            }

            ColumnLayout {
                RadioButton {
                    id: language_chinese
                    text: qsTr("中文")
                    checked: true
                    onCheckedChanged: {
                        if (checked)
                        {
//                            ischeckedchange(language_chinese.checked, language_tradition.checked,
//                                            language_english.checked, language_russian.checked)
                            ischeckedchange(language_chinese, language_tradition,
                                            language_english, language_russian)
                        }
                    }
                }
                RadioButton {
                    id: language_tradition
                    text: qsTr("繁体")
                    onCheckedChanged: {
                        if (checked)
                        {
                            ischeckedchange(language_chinese, language_tradition,
                                            language_english, language_russian)
                        }
                    }
                }
            }
            ColumnLayout {
                RadioButton {
                    id: language_english
                    text: qsTr("英语")
                    onCheckedChanged: {
                        if (checked)
                        {
                            ischeckedchange(language_chinese, language_tradition,
                                            language_english, language_russian)
                        }
                    }
                }
                RadioButton {
                    id: language_russian
                    text: qsTr("俄语")
                    onCheckedChanged: {
                        if (checked)
                        {
                            ischeckedchange(language_chinese, language_tradition,
                                            language_english, language_russian)
                        }
                    }
                }
            }
        }
    }
    function ischeckedchange(checked0, checked1, checked2, checked3)
    {
        idstrList[0] = checked0;
        idstrList[1] = checked1;
        idstrList[2] = checked2;
        idstrList[3] = checked3;
        curstrList[0] = checked0.checked;
        curstrList[1] = checked1.checked;
        curstrList[2] = checked2.checked;
        curstrList[3] = checked3.checked;

        for (var i = 0; i < 4; i++)
        {
            if (prestrList[i] === false && curstrList[i] === true)
            {
                console.log(i + "is true")
                for (var k = 0; k < 4; k++)
                {
                    if (k !== i) //当前的radiobutton被选中时，其他的按钮必须被置为checked = false
                    {
                        idstrList[k].checked = false;
                    }
                }
            }
            console.log("curstrList" + i + " =", curstrList[i] )
        }
        for (var j = 0; j < 4; j++)
        {
            if (prestrList[j] === true)
            {
                console.log(j + " is true")
            }
            console.log("curstrList" + j + " =", curstrList[j] )
        }
        recordchecked(checked0, checked1, checked2, checked3)
    }

    function recordchecked(checked0, checked1, checked2, checked3)
    {
        prestrList[0] = checked0.checked;
        prestrList[1] = checked1.checked;
        prestrList[2] = checked2.checked;
        prestrList[3] = checked3.checked;
        for (var j = 0; j < 4; j++)
        {
//            if (prestrList[j] === true)
//            {
//                console.log(j + " "+ "is true")
//            }
            console.log("prestrList" + j + " =", prestrList[j] )
        }
    }

    Component.onCompleted: {
        recordchecked(language_chinese, language_tradition,
                        language_english, language_russian)
    }
}
