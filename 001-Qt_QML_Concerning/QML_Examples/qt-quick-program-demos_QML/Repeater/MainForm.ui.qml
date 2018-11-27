import QtQuick 2.3

Rectangle {
    property alias mouseArea: mouseArea

    width: 360
    height: 360

    MouseArea {
        id: mouseArea
        anchors.fill: parent
    }

    Grid {                         // Grid定位器
        x: 25
        y: 25
        spacing: 4
        //用重复器为Grid添加元素成员
        Repeater {                             // (a)
            model: 16                          //要创建的元素成员的个数
            Rectangle {                        //成员皆为矩形元素
                id:btn
                width: 48
                height: 48
                color: "aqua"
                Text {                          //  显示矩形编号
                    id: numtext
                    anchors.centerIn: parent
                    color: "black"
                    font.pointSize: 20
                    text: index                  // (b)
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        btn.color = "red"
                        numtext.color = "green"
                    }
                }
            }
        }
    }
}
/*
(a)Repeater {……}:重复器，作为Grid的数据提供者，它可以创建任何QML基本的可视元素。Repeater会按照其model属性定义的个数循环生成子元素,
故上面代码重复生成16个Rectangle

(b)text：index：Repeater会为每一个子元素注入一个index属性，作为当前的循环索引（本例中是0～15)。可以在子元素定义中直接使用这个属性,
故这里用它给Text的text属性赋值
*/
