import QtQuick 2.3

Rectangle {
    property alias mouseArea: mouseArea

    width: 360
    height: 360

    MouseArea {
        id: mouseArea
        anchors.fill: parent
    }

    Row {                              //(a)
        x: 25
        y: 25
        spacing: 10                      //元素间隔为10像素
        layoutDirection: Qt.RightToLeft  //元素从右向左排列
        RedRectangle { }
        GreenRectangle { }
        BlueRectangle { }
    }

    Column {                            //(b)
        x: 25
        y: 120
        spacing: 2
        //以下添加被Colunmn定位的元素成员
        RedRectangle { }
        GreenRectangle { }
        BlueRectangle { }
    }

    Grid {                             //(c)
        x: 140
        y: 120
        columns: 3                     //每行3个
        spacing: 5
        // 以下添加被 Grid定位的元素成员
        BlueRectangle { }
        BlueRectangle { }
        BlueRectangle { }
        BlueRectangle { }
        BlueRectangle { }
    }
}

/*
(a) Row {……}:Row 将被其定位的元素成员都放置在一行的位置，所有元素之间间距相等(有Spacing属性设置),
顶端保持对齐。layoutDirection 属性设置元素的排列顺序，可取值Qt.LeftToRight（默认，从左到右),
Qt.RightToLeft(从右到左)

(b) Column {……}: Column 将元素成员按照加入的顺序从上到下在同一列排列出来，同样由spacing属性指定
元素间距，所有元素靠左对齐

(c) Grid {……} Grid将其元素成员排列为一个网格，默认从左到右排列，每行4个元素。可通过设置rows和columns
属性自定义行和列的数值，如果二者有一个不显式设置，则另一个会根据元素成员的总数计算出来。例如：本例中
column设置为3，一共放入5个蓝色矩形，行数就会自动计算为2
*/
