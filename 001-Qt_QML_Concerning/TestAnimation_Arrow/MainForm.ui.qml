import QtQuick 2.5

Item {
    property string imagesource: 'images/kone_arrow_down.png'
    id: showarea
    width: 156;
    height: 191
    clip: true

Item {
    id: id_scrollArrow
    Image {
        id: imagefirstpos
        width: showarea.width; height: showarea.height
        source: imagesource
    }
    Image {
        id: imagesecondpos
        source: imagesource
        y: showarea.height
    }
}

    NumberAnimation {
        id: animaUp
        target: id_scrollArrow
        property: "y"
        duration: 1000
        easing.type: Easing.InOutQuad
    }

    NumberAnimation {
        id: animaDw
        target: id_scrollArrow
        property: "y"
        duration: 1000
        easing.type: Easing.InOutQuad
    }

}

