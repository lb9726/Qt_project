import QtQuick 2.0

Canvas {
    width: 400
    height: 300
    id: root
    property var arrowPicture: "qrc:///image/arrow/1.png"

    onPaint: {
        var ctx = getContext("2d");
        ctx.drawImage(arrowPicture, 0, 0);

    }

    Component.onCompleted:
    {
        loadImage(arrowPicture);
    }
    onImageLoaded: {
        requestPaint();
    }
}
