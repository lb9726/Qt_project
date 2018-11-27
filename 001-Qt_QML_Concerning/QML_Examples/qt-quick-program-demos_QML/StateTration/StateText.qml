import QtQuick 2.0

Text {                              // 这是一个具有状态改变能力的Text的元素
    id: stext
    color: "grey"                   // 初始文字显示为灰色
    font.family: "Helvetica"        // 字体
    font.pointSize: 12              // 初始化字号12
    font.bold: true                 // 加粗
    MouseArea {                     // 能接受鼠标单击
        id: mArea
        anchors.fill: parent
    }
    states: [                         //(a)
        State {                       //(b)
            name: "hightlight"        //(c)
            when: mArea.pressed       //(d)
            PropertyChanges {         //(e)
                target: stext
                color: "red"          // 单词变红
                font.pointSize: 25    // 字号放大
                style: Text.Raised    // 以艺术字呈现
                styleColor: "red"
            }
        }
    ]

    transitions: [                   // (f)
        Transition {
            PropertyAnimation {
                duration: 1000
            }
        }
    ]
}
/*
(a)states: [……]：states属性包含了该元素所有状态的列表，要创建一个状态，
就向states中添加一个State对象，如果元素只有一个状态，也可省略方括号[]。

(b)State {……}： 状态对象，它定义了在该状态中要进行的所有改变，可以指定被改变的属性或创建PropertyChanges 元素，
也可以修改其他对象的属性(不仅仅是拥有该状态对象）。State 不仅限于对属性值进行修改，它还可以：
    使用StateChangeScript 运行一个脚本。
    使用PropertyChanges为一个对象重写现有的信号处理器。
    使用PropertyChanges为一个元素重定义父元素。
    使用AnchorChanges修改锚的值。

(c)name：“highlight”： 状态名称，每一个状态对象都有一个在本元素唯一的明晨，默认状态的状态名称为空字符串。
要改变一个元素的当前状态，可以将其state属性设置为要改变到的状态的名称

(d)when： mArea.pressed： when 属性设定了鼠标被按下时从默认状态进入该状态，释放鼠标则返回默认状态。
所有的QML可视元素都有一个默认状态，在默认状态中包含了该元素所有的初始化属性值（如本例为Text元素最初设置的属性值），
一个元素可以为其state 属性指定一个空字符串来明确地将其设置为默认状态。例如，这里如果不适用when属性，代码可以写为：
……
Text {
    id: stext
        ……
        MouseArea {
            id: MArea
            anchors.fill: parent
            onpressed: stext.state = "highlight"
            // 按下鼠标状态切换为“hightlight"
            onReleased: stext.state = ""  // 释放鼠标回到默认初始状态
        states: {
            State {
                name: "highlight"
                propertyChanges {
                    ……
                }
            }
        }
        …
    }
    很明显，使用when属性比使用信号处理器来分配状态更加简单，更符合QML声明式的风格。
    所以建议在这种情况下还是使用when属性来控制状态的切换。

(e)PropertyChanges {……}：在用户定义的状态中一般使用PropertyChanges（属性改变）元素给出状态切换时对象的各属性分别要变到的目标值，
其中指明target属性为stext，即对Text元素本身应用属性改变的动画

(f)transitions：[Transition { …… }]: 元素在不同的状态间改变时使用切换（transitions)来实现动画效果，
切换用来设置当状态改变时的动画，要创建一个切换，需要定义一个Transition对象，然后将其添加到元素的transitions属性。
在本例中，当Text元素变到“highlight”状态时，Transition将被触发，切换的PropertyAnimation 再设置任何from和to的属性的值，
因为在状态改变的开始之前和结束之后切换都会自动设置这些值。
*/
