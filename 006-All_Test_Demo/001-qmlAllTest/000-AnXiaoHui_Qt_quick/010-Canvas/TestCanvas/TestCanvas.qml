import QtQuick 2.0

Canvas {
    width: 400
    height: 240

    onPaint: {
        var ctx = getContext("2d");
        ctx.lineWidth = 2;
        ctx.strokeStyle = "red"; // 边框的颜色 strokeStyle 为颜色属性
        ctx.fillStyle = "blue";  // 填充的颜色
        ctx.beginPath();
        ctx.rect(100, 80, 120, 80);
        ctx.fill();
        ctx.stroke();
    }
}
