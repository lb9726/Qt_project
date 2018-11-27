import QtQuick 2.3

Rectangle {
    property alias mouseArea: mouseArea

    width: 360
    height: 360

    MouseArea {
        id: mouseArea
        anchors.fill: parent
    }

    Text {                       // 普通文本
        x: 60
        y: 100
        color: "green"           //设置颜色
        font.family: "Helvetica" //设置字体
        font.pointSize: 24       //设置字号
        text: "Hello Qt Quick"   //输出文字内容
    }

    Text {                       //富文本
        x: 60
        y: 140
        color: "green"
        font.family: "Helvetica"
        font.pointSize: 24
        text: "<b>Hello </b> <i>Qt Quick!</i>"  //(a)
    }

    Text {                             //带样式的文本
        x: 60
        y: 180
        color: "green"
        font.family: "Helvetica"
        font.pointSize: 24
        style: Text.Outline
        styleColor: "blue"       // (b)
        text: "Hello Qt Quick!"
    }

    Text {                              //带省略的文本
        width: 200                      //限制文本宽度
        color: "green"
        font.family: "Helvetica"
        font.pointSize: 24
        horizontalAlignment: Text.AlignLeft  //在窗口中左对齐
        verticalAlignment: Text.AlignTop     //在窗口中顶端对齐
        elide: Text.ElideRight               //(c)
        text: "Hello Qt Quick!"
    }

    Text {
        width: 200
        y: 30
        color: "green"
        font.family: "Helvetica"
        font.pointSize: 24
        horizontalAlignment: Text.AlignLeft
        wrapMode: Text.WrapAnywhere      // (d)
        text: "Hello Qt Quick!"
    }
}
/*
(a)text: "<b>Hello </b> <i>Qt Quick!</i>":Text元素支持用HTML类型标记定义富文本，它有一个textForm属性，默认值为
Text.RichText(输出富文本);若显式地指定为Text.PlainText,则会输出纯文本（连同HTML标记一起作为字符输出）

(b)style:Text.Outline;styleColor:"blue": style 属性设置文本的样式，支持的文本样式有Text.Normal，Text.Outline,
Text.Raised和Text.Sunken;styleColor属性设置样式的颜色，这里是蓝色

(c)elide：Text.ElideRight: 设置省略文本的部分内容来适合Text的宽度，如果没有对Text明确设置width值，则elide属性将不起
作用。elide可取的值有Text.ElideNone（默认，不省略),Text.ElideLeft（从左边省略),Text.ElideMiddle(从中间省略)和Text.ElideRight（从右边省略)

(d)wrapMode:Text.WrapAngwhere:如果不希望使用elide省略显示方式，还可以通过wrapMode、属性指定换行模式，本例中设为Text.WrapAnyWhere，
即只要达到边界（哪怕在一个单词的中间)都会进行换行；若不想这么做，可设置为Text.WordWrap只在单词边界换行
*/
