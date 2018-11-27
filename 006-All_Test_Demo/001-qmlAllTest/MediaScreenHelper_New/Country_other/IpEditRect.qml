import QtQuick 2.7
import QtQuick.Layouts 1.0
import QtQuick.Controls.Styles 1.3
import QtQuick.Controls 1.3

Rectangle {
    property alias edit1Text: edit1.text
    property alias edit2Text: edit2.text
    property alias edit3Text: edit3.text
    property alias edit4Text: edit4.text
    property int editwidth: 60
    width: 350
    height: 35
    border.color: "grey"
    border.width: 1
    radius: 5
    RowLayout {
        anchors.centerIn: parent
        TextField {
            id: edit1
            validator: IntValidator {bottom: 0; top: 255}
            font.family: "KONE Information_v12"
            style: TextFieldStyle { // 自定义样式
                background: Rectangle {
                    radius: 1
                    implicitWidth: editwidth
                    implicitHeight: 20
                }
            }
            onEditingFinished: {
                console.log("edit1Text = ", edit1Text);
            }
        }
        Label {
            text:"."
        }
        TextField {
            id: edit2
            validator: IntValidator {bottom: 0; top: 255}
            font.family: "KONE Information_v12"
            style: TextFieldStyle { // 自定义样式
                background: Rectangle {
                    radius: 1
                    implicitWidth: editwidth
                    implicitHeight: 20
                }
            }
            onEditingFinished: {
                console.log("edit2Text = ", edit2Text);
            }
        }
        Label {
            text:"."
        }
        TextField {
            id: edit3
            validator: IntValidator {bottom: 0; top: 255}
            font.family: "KONE Information_v12"
            style: TextFieldStyle { // 自定义样式
                background: Rectangle {
                    radius: 1
                    implicitWidth: editwidth
                    implicitHeight: 20
                }
            }
            onEditingFinished: {
                console.log("edit3Text = ", edit3Text);
            }
        }
        Label {
            text:"."
        }
        TextField {
            id: edit4
            validator: IntValidator {bottom: 0; top: 255}
            font.family: "KONE Information_v12"
            style: TextFieldStyle { // 自定义样式
                background: Rectangle {
                    radius: 1
                    implicitWidth: editwidth
                    implicitHeight: 20
                }
            }
            onEditingFinished: {
                console.log("edit4Text = ", edit4Text);
            }
        }
    }
//    Component.onCompleted: {
//        console.log("I am IpEditRect");
//    }

    function getAddress()
    {
        var str = "";
        if (edit1Text === "" && edit2Text === ""
            && edit3Text === "" && edit4Text === "")
        {
            str = "0.0.0.0";
        }
        else
        {
            if (edit1Text === "" )
            {
                str = str + "0";
            }
            else
            {
                str = str + edit1Text;
            }
            if (edit2Text === "")
            {
                str = str +".0";
            }
            else
            {
                str = str + "." + edit2Text;
            }
            if (edit3Text === "")
            {
                str = str +".0";
            }
            else
            {
                str = str + "." + edit3Text;
            }
            if (edit4Text === "")
            {
                str = str +".0";
            }
            else
            {
                str = str + "." + edit4Text;
            }
        }
        console.log("str = ", str);
        return str;
    }
}

