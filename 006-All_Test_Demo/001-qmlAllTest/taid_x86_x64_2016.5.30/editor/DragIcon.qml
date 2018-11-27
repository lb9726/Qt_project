import QtQuick 2.5

Image {
    id: dragItem
    property bool created: false
    property string image: ""
    property int toHeight
    source: image
    width: 50
    height: 50
}
