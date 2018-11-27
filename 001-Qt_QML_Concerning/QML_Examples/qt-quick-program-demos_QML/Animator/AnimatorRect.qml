import QtQuick 2.4

Rectangle {
    width: 80
    height: 80
    color: "green"
    XAnimator on x {
        from: 10;
        to: 100;
        duration: 7000
        loops: Animator.Infinite
    }

    YAnimator on y {
        from: 10;
        to: 100;
        duration: 7000
        loops: Animator.Infinite
    }

    ScaleAnimator on scale {    //ScaleAnimation 类型改变了一个元素的尺度因子，产生使元素大小尺寸缩放的动画
        from: 0.1;
        to: 1;
        duration: 7000
        loops: Animator.Infinite
    }

    RotationAnimator on rotation { //RotationAnimation 动画元素允许设定图形旋转的方向，指定from和to属性，师矩形360度旋转
        from: 0;
        to: 360;
        duration: 7000
        loops: Animator.Infinite
    }

    OpacityAnimator on opacity {
        from: 0;
        to: 1;
        duration: 7000
        loops: Animator.Infinite
    }
}

