import QtQuick 2.0

Rectangle {                 //将Rectangle自定义为按钮
    id: btn
    width: 100
    height: 62              //按钮尺寸
    color: "teal"           //按钮颜色
    border.color: "aqua"    //按钮边界色
    border.width: 3         //按钮边界宽度

    Text {                  //Text元素作为按钮文本
        id: lable
        anchors.centerIn: parent
        font.pointSize: 16
        text: "开始"
    }

    MouseArea {                        // MouseArea 对象作为按钮单击事件响应区
        anchors.fill: parent
        onClicked: {                   //响应单击事件代码
            lable.text = "按钮已按下!"
            lable.font.pointSize = 11  //改变按钮文本和字号
            btn.color = "aqua"         //改变按钮颜色
            btn.border.color = "teal"  //改变按钮边界色

            /*改变changingRect1的左锚属性*/         // (a)
            chgRect1.anchors.left = undefined
            chgRect1.anchors.right = rRect.right

            /*改变changingRect2的右锚属性*/         // (b)
            chgRect2.anchors.right = rRect.right
            chgRect2.anchors.left = undefined
        }
    }
}
/*该文件将一个普通的矩形元素“改造”成按钮，并且封装了按钮的文本，颜色，边界等属性，同时定义了它在响应用户单击时行为*/
/*
(a) 改变changingRect1的右锚属性：这里用“chgRect1.anchors.left = undefined" 先解除其左锚属性的定义，然后再定义右锚属性，
执行后，该矩形便会移动到与redRect（第一行最右边的红矩形)右对齐

（b)改变changingRect2的右锚属性：这里是先用“chgRect2.anchors.right = rRect.right” 指定右锚属性,由于此时元素属性尚未解除,
执行后，矩形位置并不会移动，而是宽度自动“拉长" 到与redRect右对齐，之后即使再解除左锚属性也无济于事，故用户在编程改变布局时，
一定要先将元素旧有的锚解除，新设置的锚才能生效
*/
