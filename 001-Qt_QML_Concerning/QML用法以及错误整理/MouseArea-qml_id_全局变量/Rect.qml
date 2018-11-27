import QtQuick 2.0

Rectangle {
    color: "teal"
    width: 50
    height: 50

    MouseArea {                            // 定义MouseArea 元素处理鼠标事件
        anchors.fill: parent               // 事件响应充满整个矩形
        /* 拖拽属性设置 */
        drag.target: parent  //
        drag.axis: Drag.XAxis
        drag.minimumX: 0
        drag.maximumX: 360 - parent.width
        acceptedButtons: Qt.LeftButton|Qt.RightButton  // (b)
        onClicked: {  /*处理鼠标事件的代码*/
            if (Qt.RightButton === mouse.button) {    // (c)
                parent.color = "blue";  // 设置矩形为蓝色并缩小尺寸
                parent.width -= 5;
                parent.height -= 5;
            }else if ((Qt.LeftButton === mouse.button) && (mouse.modifiers & Qt.ShiftModifier)) {
                // 把矩形重新设为蓝绿色并恢复原来的大小
                parent.color = "teal";
                parent.width = 50;
                parent.height = 50;
            }else {
                // 设置矩形为绿色并增大尺寸
                parent.color = "green";
                parent.width += 5;
                parent.height += 5;
            }
       }
    }
}

/*

(a)拖拽属性设置：MouseArea中的drag分组属性提供一个使得元素可以拖拽的简便方法。drag.target 属性用来指定拖动的元素的id(这里为parent表示拖动的就是所在元素本身)
:drag.active 属性获取元素当前是否正在被拖动的信息；drag.axis属性用来指定拖动的方向，可以是水平方向（Drag.XAxis），垂直方向（Drag.YAxis)或者两个方向都可以(
Drag.XandYAxis)；drag.minimumX和drag.maximumX 限制了元素在指定方向上拖动的范围

(b) acceptedButtons:Qt.LeftButton|Qt.RightButton: MouseArea 所能接受的鼠标按键，可取的值有Qt.LeftButton(鼠标左键),Qt.RightButton(鼠标右键)和Qt.MiddleButton（鼠标中键）

(c)mouse.button: 为MouseArea 信号中所包含的鼠标事件参数，其中mouse为鼠标事件对象，可以通过它的x和y属性获取当前的位置；通过button属性获取按下的按键

(d)mouse.modifiers & Qt.ShiftModifier：通过modifiers属性可以获取按下的键盘修饰符，modifiers的值由多个按键进行位组合而成，在使用时需要将modifiers与这些特殊的按键进行按位
与来判断按键，常用的按键有Qt.NoModifier（没有修饰键），Qt.ShiftModifier(一个Shift键),Qt.ControlModifier(一个Ctrl键），Qt.AltModifier（一个Alt键）

*/
