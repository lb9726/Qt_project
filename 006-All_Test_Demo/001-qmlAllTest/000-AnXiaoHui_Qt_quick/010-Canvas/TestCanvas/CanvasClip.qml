import QtQuick 2.0
/*
裁切
Context2D的clip()方法，让我们能够根据当前路径包围的区域来裁切后续的绘图操作，在此区域之外的图像
都会被毫不留情地丢掉。使用如下步骤进行裁切
1. beginPath()
2. lineTo(), arc(), bezierCurveTo(), moveTo(), closePath()等创建路径。
3. 调用clip() 确定裁切区域
4. 绘图， 如何使用drawImage()
*/
Canvas {
    width: 640
    height: 640
    contextType: "2d"
    property var comicRole: "http://img.zcool.cn/community/01635d571ed29832f875a3994c7836.png@900w_1l_2o_100sh.jpg"
    onPaint: {
        context.lineWidth = 2;
        context.strokeStyle = "blue";
        context.fillStyle = Qt.rgba(0.3, 0.5, 0.7, 0.3);

        context.save();
        context.beginPath();
        context.arc(180, 150, 80, 0, Math.PI * 2, true);
        context.moveTo(180, 230);
        context.lineTo(420, 280);
        context.lineTo(160, 320);
        context.closePath();
        context.clip();
        context.drawImage(comicRole, 0, 0, 600, 600, 0, 0, 400, 400);// qrc:/CanvasClip.qml:29: Error: drawImage(), index size error
        context.stroke();
        context.fill();

        context.rotate(Math.PI / 5);
        context.font = "italic bold 32px serif";
        context.fillStyle = "red";
        context.fillText("the text will clipped!", 100, 70);
        context.restore();
    }
    Component.onCompleted: {
        loadImage(comicRole);
    }
    onImageLoaded: {
        requestPaint();
    }
}
