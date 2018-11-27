import QtQuick 2.0
/*
Context2D是QML中负责2D绘图的对象，与Canvas结合使用。有两种使用Context2D对象的方式，
上面的例子中在onPaint信号处理器中调用了getContext("2d")获取Context2D对象的方式，
这是一种方式。还有一种方式，当我们设置了Canvas对象的contextType属性(2D绘图时取值为"2d")后，
context属性就会保存一个可用的Context2D对象。所以示例可以修改为
*/
Canvas {
    width: 400;
    height: 240;
    contextType: "2d";

    onPaint: {
        context.lineWidth = 2;
        context.strokeStyle = "red";
        context.fillStyle = "blue";
        context.beginPath();
        context.rect(100, 80, 120, 80);
        context.fill();
        context.stroke();
    }
}
