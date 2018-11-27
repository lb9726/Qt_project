import QtQuick 2.3

Rectangle {
    property alias mouseArea: mouseArea

    width: 360
    height: 360

    MouseArea {
        id: mouseArea
        anchors.fill: parent
    }

    Row {              //所有图标成一行横向排列
        x: 50
        y: 50
        spacing: 30
        Rectangle {
            id: music   //第一个矩形元素（“音乐”图标）
            width: 100
            height: 100
            radius: 6
            color: focus ? "red" : "lightgray"   // 被选中（获得焦点）时显示红色，否则变灰
            scale: focus ? 1: 0.8                // 被选中（获得焦点）时图标变大
            focus: true                          //初始时选中音乐图标
            KeyNavigation.tab: play    //(a)
            /* 移动图标位置*/            //(b)
            Keys.onUpPressed: music.y -= 10    // 上移
            Keys.onDownPressed: music.y += 10  // 下移
            Keys.onLeftPressed: music.x -= 10  // 左移
            Keys.onRightPressed: music.x += 10 // 右移

            Text {                             // 图标上显示文本
                anchors.centerIn: parent
                color:parent.focus ? "black" : "gray"  // 被选中（获得焦点）时显示黑色，否则变灰
                font.pointSize: 20                //字体大小
                text: "音乐"                       //文字内容为“音乐”
            }
        }

        Rectangle {                       //第二个矩形元素（“游戏”图标）
            id: play
            width: 100; height: 100
            radius: 6
            color: focus ? "green"  : "lightgray"
            scale: focus ? 1: 0.8
            KeyNavigation.tab: movie          //焦点转移到“影视”图标
            /* */
            Keys.onUpPressed: play.y -= 10    //
            Keys.onDownPressed: play.y += 10  //
            Keys.onLeftPressed: play.x -= 10  //
            Keys.onRightPressed: play.x += 10 //

            Text {
                anchors.centerIn: parent
                color:parent.focus ? "black" : "gray"
                font.pointSize: 20
                text: "游戏"
            }
        }

        Rectangle {                            //第三个矩形元素（“影视”图标）
            id: movie
            width: 100; height:  100
            radius: 6
            color:focus? "blue":"lightgray"
            scale:focus? 1 : 0.8
            KeyNavigation.tab: music           //焦点转移到“音乐”图标
            /* */
            Keys.onUpPressed: movie.y -= 10    //
            Keys.onDownPressed: movie.y += 10  //
            Keys.onLeftPressed: movie.x -= 10  //
            Keys.onRightPressed: movie.x += 10 //

            Text {
                anchors.centerIn: parent
                color:parent.focus ? "black" : "gray"
                font.pointSize: 20
                text: "影视"
            }
        }
    }
}

/*
(a)KeyNavigation.tab: play： QML中的KeyNavigation 元素是一个附加属性，可以用来实现使用方向键或【Tab】键来进行元素导航。它的子属性有backtab,down,left,
priority,right,tab和up等，本例用其tab属性设置焦点转移次序，”keynavigation.tab:play“，表示按下【Tab】键焦点转移到id为play 的元素（”游戏“图标）

(b) 移动图标位置：这里使用keys属性来进行按下方向键后的事件处理，它也是一个附加属性，对QML所有的基本可视元素均有效。Keys属性一般与focus属性配合使用，只有
当foucs值为true时它才起作用，由Keys属性获取相应键盘事件的类型，进而决定所要执行的操作。本例Keys.onUpPressed 表示方向键被按下的事件，相应地执行该元素y坐标
-10（上移）操作，其余方向的操作与之类同
*/
