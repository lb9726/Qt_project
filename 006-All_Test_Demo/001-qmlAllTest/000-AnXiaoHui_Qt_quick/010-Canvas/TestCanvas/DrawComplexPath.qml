import QtQuick 2.0

Canvas {
    width: 400
    height: 240

    id: root
    onPaint: {
        var ctx = getContext("2d");
        ctx.lineWidth = 2;
        ctx.strokeStyle = "red";

        ctx.font = "42px sans-serif";
        var gradient = ctx.createLinearGradient(0, 0, width, height);
        gradient.addColorStop(0.0, Qt.rgba(0, 1, 0, 1));
        gradient.addColorStop(1.0, Qt.rgba(0, 0, 1, 1));
        ctx.fillStyle = gradient;
        ctx.beginPath();
        ctx.moveTo(4, 4);
        ctx.bezierCurveTo(0, height - 1, width - 1, height / 2.0, width / 4.0, height / 4);

        ctx.lineTo(width / 2.0, height / 4.0);
        ctx.arc(width * 5 / 8.0, height / 4.0, width / 8.0, Math.PI, 0, false);
        ctx.ellipse(width * 11 / 16.0, height / 4.0, width / 8.0, height / 4.0);
        ctx.lineTo(width / 2.0, height * 7 / 8.0);
        ctx.text("Complex Path", width / 4.0, height * 7 / 8.0);
        ctx.fill();
        ctx.stroke();
    }
}
