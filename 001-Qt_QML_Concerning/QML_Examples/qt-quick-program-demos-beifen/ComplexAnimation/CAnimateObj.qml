import QtQuick 2.4   //使用最新版Qt Quick 2.4以支持Animator

Rectangle {
    id: rect
    width: 240
    height: 300
    color: "grey"
    SequentialAnimation on x {                // (a)
        id: rectAnim
        running: false
        loops: Animation.Infinite
        /*实现往返运动*/
        NumberAnimation { from: 0; to: 500; duration: 8000; easing.type: Easing.InOutQuad }
        NumberAnimation { from: 500; to: 0; duration: 8000; easing.type: Easing.InOutQuad }
        PauseAnimation { duration: 1000 }
    }

    Image {                                     /// 图像元素显示照片
        id: img
        source: "images/zhou.jpg"
        anchors.horizontalCenter: parent.horizontalCenter    // 照片沿垂直中线下落
        y: 0                                                 // 初始时位于顶端
        scale: 0.1                                           // 大小仅原尺寸的十分之一
        opacity: 0                                           // 初始透明度为0（不可见）
        rotation: 45                                         // 初始放置的角度
    }

    SequentialAnimation {                                   // (b)
        id: imgAnim
        loops: Animation.Infinite
        ParallelAnimation {                                 // (c)
            ScaleAnimator { target: img; to: 1; duration: 1500 }
            OpacityAnimator { target: img; to: 1; duration: 2000 }
            RotationAnimator { target: img; to: 360; duration: 1500 }
            NumberAnimation {
                target: img
                property: "y"
                to: rect.height - img.height                // 运动到矩形区的底部
                easing.type: Easing.OutBounce          // 为造成照片落地后又“弹起”的效果
                duration:  5000
            }
        }
        PauseAnimation { duration: 2000 }
	
        ParallelAnimation {
            NumberAnimation {
                target: img
                property: "y"
                to: 0
                easing.type: Easing.OutQuad
                duration: 1000
            }
            OpacityAnimator { target: img; to: 0; duration: 1000 }
        }
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            rectAnim.running = true       // 开启水平方向（矩形往返) 动画
            imgAnim.running = true        // 开启垂直方向（照片掉落）动画
        }
    }
}

/*
多个单一的动画可以组合成一个复合的动画，这可以使用ParellelAnimation或者SequentialAnimation 动画组元素来实现。
在ParallelAnimation 中的动画会同时（并行）运行，而SequentialAnimation 中的动画则会一个接一个的（串行）地运行。
要想运行复杂的动画，可以在一个动画组中进行设计。
*/
/*
(a) SequentialAnimation on x {…}：创建了SequentialAnimation来串行地运行3个动画，即NumberAnimation（右移），NumberAnimation(左移）和PauseAnimation（停顿）。
这里，SequentialAnimation作为属性值源动画应用在Rectangle的x属性上，动画默认会在程序运行后就自动执行，为便于控制，故将其running 属性设为false改由手动
开启动画。因为SequentialAnimation 是应用在x属性上的，所以在组中的独立动画也都会自动应用在x属性上

(b)SequentialAnimation {…}： 这个SequentialAnimation 并未定义在任何属性上，故其中的各个子动画元素必须以target和property分别指明要应用到的目标元素和属性，
也可以使用Animator动画（这种情况只须给出应用的目标元素即可）。动画组可以嵌套，本例就是一个典型的嵌套动画，这个串行动画由两个ParellelAnimation动画以及他们
之间的PuaseAnimation组成
(c)ParellelAnimation {…} 并行动画组，其中各个子动画元素同时运行，本例包含4个独立的子动画，即ScaleAnimator（使照片尺寸变大），OpacityAnimator（照片由隐而见），
RotationAnimation（照片旋转角度），NumberAnimation（照片位置从上往下）…… 它们并行地执行，于是产生出照片旋转着下落的视觉效果

*/
