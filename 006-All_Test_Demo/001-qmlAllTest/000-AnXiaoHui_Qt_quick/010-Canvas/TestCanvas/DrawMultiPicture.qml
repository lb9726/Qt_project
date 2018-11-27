import QtQuick 2.0

Canvas {
    width: 400
    height: 300
    id: root
    property var arrowPicture: "qrc:///image/arrow/1.png";
    property var poster:
        "http://img.zcool.cn/community/01635d571ed29832f875a3994c7836.png@900w_1l_2o_100sh.jpg"

    onPaint: {
        var ctx = getContext("2d");
        ctx.drawImage(arrowPicture, 0, 0);
        ctx.drawImage(poster, 120, 0);
    }
    Component.onCompleted: {
        loadImage(arrowPicture);
        loadImage(poster);
    }
    onImageLoaded: {
        requestPaint();
    }
}
