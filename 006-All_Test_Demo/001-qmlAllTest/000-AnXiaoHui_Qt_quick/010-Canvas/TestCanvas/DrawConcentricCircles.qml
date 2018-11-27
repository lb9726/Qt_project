import QtQuick 2.2

Canvas {
    width: 300
    height: 300
    contextType: "2d"

    onPaint: {
        context.lineWidth = 2;
        context.strokeStyle = "blue";
        context.fillStyle = "red";
        context.save();

        context.translate(width / 2.0, height / 2.0);
        context.beginPath();
        context.arc(0, 0, 30, 0, Math.PI * 2); // 画圆圈
        context.arc(0, 0, 50, 0, Math.PI *2);
        context.arc(0, 0, 70, 0, Math.PI * 2);
        context.arc(0, 0, 90, 0, Math.PI * 2);

        context.stroke();
        context.restore();

        context.save();
        context.translate(width / 2.0, 30);
        context.font = "26px serif";
        context.textAlign = "center";
        context.fillText("concentric circles", 0, 0);
        context.restore();
    }
}

/*
如你所见，四个同心圆圈以界面的中心点为圆心，而我在绘制时却指定了(0, 0)为圆心，这是因为使用了
translate()将坐标系的圆心(0, 0)从画布左上角变换到了画布中心，然后我们在绘图时引用的原点(0, 0)
就不再是左上角了。
平移变换，绘图操作完成后，应当调用restore()来恢复当前的画布状态，否则发生重绘时(比如用户拖动窗口改变大小),
你就看不见绘制的图元了。而要restore(),必须先save(),正如示例代码中写的那样。
translate(real x, real y)方法平移画布原点，x, y两个参数是相对于当前原点的偏移量。这点一定切记，如果你
需要不断地变换原点才能实现预期效果，那就要正确地计算每次平移变换的偏移量。
为了是Text "concentric circle" 在画布上方居中显示，先恢复画布原点到左上角，然后save(),接着变换到(width/2, 30)，
接着设置了textAlign属性为"center"，最后调用了fillText()在原点处绘制文本，画完后再次调用restore()
*/
