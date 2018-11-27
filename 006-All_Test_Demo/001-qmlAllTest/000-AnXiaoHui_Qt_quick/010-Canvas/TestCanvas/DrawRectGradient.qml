import QtQuick 2.0
/*
使用Context2D绘制路径的一般步骤
1. beginPath()
2. 调用moveTo(),lineTo(),arcTo(), rect(),quadracticCurveTo(),arc(),bezierCurveTo()等可以构造路径元素的方法。
3. 调用fill()或stroke().
如你所见，我们地一个使用Canvas的示例正是这么干的，它调用rect()方法在路径上添加一个矩形，然后调用fill()方法使用fillStyle
保存的颜色来填充矩形，调用stroke()方法结束路径的绘制并使用strokeStyle保存的颜色描画路径的边线。
Context2D的lineWidth属性，用来设置画笔的宽度。
fillStyle与Qt C++中的QBrush类似，保存用于填充图元的画刷。它可以是一个颜色值，也可以是CanvasGradient
或者是CanvasPattern对象。

当使用颜色时，支持以下语法：
rgb(red,green,blue), 比如：rgb(255, 100, 55)或rgb(100%, 70%, 30%)。
rgba(red, green, blue, alpha),比如rgba(255, 100, 55, 1.0)或rgba(100%, 70%, 30%, 0.5)。
#RRGGBB,比如 #00FFCC。
Qt.rgba(red, green, blue, alpha), 比如Qt.rgba(0.3, 0.7, 1, 1.0)类似的还有Qt.hsla(),Qt.lighter()等。
Context2D的createLinearGradient()方法用于创建一个线性渐变对象，createRadialGradient()方法可以创建一个放射渐变对象
类型为CanvasGradient。CanvasGradient对象的addColorStop()可以添加渐变路径上的关键点的颜色

*/
Canvas {
    width: 400
    height: 240
    onPaint: {
        var ctx = getContext("2d");
        ctx.lineWidth = 2;
        ctx.strokeStyle = "red";
        var gradient = ctx.createLinearGradient(60, 50, 180, 130);
        gradient.addColorStop(0.0, Qt.rgba(1, 0, 0, 1.0));
        gradient.addColorStop(1.0, Qt.rgba(0, 0, 1, 1.0));
        ctx.fillStyle = gradient;
        ctx.beginPath();
        ctx.rect(60, 50, 120, 80);
        ctx.fill();
        ctx.stroke();

        gradient = ctx.createRadialGradient(230, 160, 30, 260, 200, 20);
        gradient.addColorStop(0.0, Qt.rgba(1, 0, 0, 1.0));
        gradient.addColorStop(1.0, Qt.rgba(0, 0, 1, 1.0));
        ctx.fillStyle = gradient;
        ctx.rect(200, 140, 80, 80);
        ctx.fill();
        ctx.stroke();
    }
}
