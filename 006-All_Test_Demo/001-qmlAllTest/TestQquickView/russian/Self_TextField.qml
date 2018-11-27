import QtQuick 2.7
import QtQuick.Layouts 1.0
import QtQuick.Controls.Styles 1.3
import QtQuick.Controls 1.3

Item {
    property int yearwidth: 70
    property int otherwidth: 40
    property alias yeartext: editYear.text
    property alias monthtext: editMonth.text
    property alias daytext: editDay.text
    property alias hourtext: editHour.text
    property alias minutetext: editMinute.text
    property alias secondtext: editSecond.text
    width: 250
    ColumnLayout
    {
        spacing: 10
        Item {
            height: 1
        }

        RowLayout {
            Item {
                width: yearwidth
                Label {
                    id: idyearText
                    anchors.centerIn: parent
                    text: "Y" // 年
                }
            }
            Item {
                width: 8
            }
            Item {
                width: otherwidth
                Label {
                    id: idMonthText
                    anchors.centerIn: parent
                    text: "M" //qsTr("月")
                }
            }
            Item {
                width: 7
            }
            Item {
                width: otherwidth
                Label {
                    id: idDayText
                    anchors.centerIn: parent
                    text: "d" // qsTr("日")
                }
            }
            Item {
                width: 6
            }
            Item {
                width: otherwidth
                Label {
                    id: idHourText
                    anchors.centerIn: parent
                    text: "h" // qsTr("时")
                }
            }
            Item {
                width: 5
            }
            Item {
                width: otherwidth
                Label {
                    id: idMinuteText
                    anchors.centerIn: parent
                    text: "m" // qsTr("分")
                }
            }
            Item {
                width: 5
            }
            Item {
                width: otherwidth
                Label {
                    id: idSecondText
                    anchors.centerIn: parent
                    text: "s"// qsTr("秒")
                }
            }
        }

        RowLayout {
            TextField {
                id: editYear
                validator: IntValidator {bottom: 0; top: 2050}
                font.family: "KONE Information_v12"
                horizontalAlignment: TextInput.AlignHCenter
                style: TextFieldStyle { // 自定义样式
                    background: Rectangle {
                        radius: 1
                        implicitWidth: yearwidth
                        implicitHeight: 30
                        border.color: "green"
                        border.width: 1
                    }
                }
                onEditingFinished: {
//                    yeartext = editYear.getText(0, editYear.length);
                    console.log("yeartext = ", yeartext);
                }
            }
            Label {
                text:"-"
            }
            TextField {
                id: editMonth
                validator: IntValidator {bottom: 1; top: 12}
                horizontalAlignment: TextInput.AlignHCenter
                font.family: "KONE Information_v12"
                style: TextFieldStyle { // 自定义样式
                    background: Rectangle {
                        radius: 1
                        implicitWidth: otherwidth
                        implicitHeight: 30
                        border.color: "green"
                        border.width: 1
                    }
                }
                onEditingFinished: {
//                    monthtext = editMonth.getText(0, editMonth.length);
                    console.log("monthtext = ", monthtext);
                }
            }
            Label {
                text:"-"
            }
            TextField {
                id: editDay
                validator: IntValidator {bottom: 0; top: 31}
                horizontalAlignment: TextInput.AlignHCenter
                font.family: "KONE Information_v12"
                style: TextFieldStyle { // 自定义样式
                    background: Rectangle {
                        radius: 1
                        implicitWidth: otherwidth
                        implicitHeight: 30
                        border.color: "green"
                        border.width: 1
                    }
                }
                onEditingFinished: {
//                    daytext = editDay.getText(0, editDay.length);
                    console.log("daytext = ", daytext);
                }
            }
            Label {
                text:"-"
            }
            TextField {
                id: editHour
                validator: IntValidator {bottom: 0; top: 23}
                horizontalAlignment: TextInput.AlignHCenter
                font.family: "KONE Information_v12"
                style: TextFieldStyle { // 自定义样式
                    background: Rectangle {
                        radius: 1
                        implicitWidth: otherwidth
                        implicitHeight: 30
                        border.color: "green"
                        border.width: 1
                    }
                }
                onEditingFinished: {
//                    hourtext = editHour.getText(0, editHour.length);
                    console.log("hourtext = ", hourtext);
                }
            }
            Label {
                text:":"
            }
            TextField {
                id: editMinute
                validator: IntValidator {bottom: 0; top: 59}
                horizontalAlignment: TextInput.AlignHCenter
                font.family: "KONE Information_v12"
                style: TextFieldStyle { // 自定义样式
                    background: Rectangle {
                        radius: 1
                        implicitWidth: otherwidth
                        implicitHeight: 30
                        border.color: "green"
                        border.width: 1
                    }
                }
                onEditingFinished: {
//                    minutetext = editMinute.getText(0, editMinute.length);
                    console.log("minutetext = ", minutetext);
                }
            }
            Label {
                text:":"
            }
            TextField {
                id: editSecond
                validator: IntValidator {bottom: 0; top: 59}
                horizontalAlignment: TextInput.AlignHCenter
                font.family: "KONE Information_v12"
                style: TextFieldStyle { // 自定义样式
                    background: Rectangle {
                        radius: 1
                        implicitWidth: otherwidth
                        implicitHeight: 30
                        border.color: "green"
                        border.width: 1
                    }
                }
                onEditingFinished: {
//                    secondtext = editSecond.getText(0, editSecond.length);
                    console.log("secondtext = ", secondtext);
                }
            }
        }
    }
    function getEditTime()
    {
        var str = "";
        if ("" === yeartext && "" === monthtext && "" === daytext
            && "" === hourtext && "" === minutetext && "" === secondtext)
        {
            str = ""
        }
        else
        {
            if ("" === yeartext)
            {
                str = str + "2018-"
            }
            else
            {
                str = str + yeartext + "-"
            }
            if ("" === monthtext)
            {
                str = str + "01-"
            }
            else
            {
                str = str + monthtext + "-"
            }
            if ("" === daytext)
            {
                str = str + "01-"
            }
            else
            {
                str = str + daytext + "-"
            }
            if ("" === hourtext)
            {
                str = str + "00:"
            }
            else
            {
                str = str + hourtext + ":"
            }
            if ("" === minutetext)
            {
                str = str + "00:"
            }
            else
            {
                str = str + minutetext + ":"
            }
            if ("" === secondtext)
            {
                str = str + "00"
            }
            else
            {
                str = str + secondtext
            }
        }
        return str;
    }

    function translateText()
    {
        idyearText.text = qsTr("年")
        idMonthText.text = qsTr("月")
        idDayText.text = qsTr("日")
        idHourText.text = qsTr("时")
        idMinuteText.text = qsTr("分")
        idSecondText.text = qsTr("秒")
    }
}
