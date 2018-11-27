import QtQuick 2.0

Canvas {
    width: 400
    height: 300
    id: root
    Image {
        id: poster
        source: "http://img.zcool.cn/community/01635d571ed29832f875a3994c7836.png@900w_1l_2o_100sh.jpg"
        visible: false
        onStatusChanged: {
            if (status === Image.Ready)
            {
                poster.visible = true;
                root.requestPaint();
            }
        }
    }

    onPaint: {
        var ctx = getContext("2d");
        ctx.drawImage(poster, 50, 0);
    }
}
