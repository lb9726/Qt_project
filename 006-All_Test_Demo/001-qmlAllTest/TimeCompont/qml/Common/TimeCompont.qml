import QtQuick 1.1

Item {
    property real itemHeight: 20
    property string title
    property alias currentIndexHour: pathViewHourTime.currentIndex
    property alias currentIndexMin: pathViewMinTime.currentIndex

    signal timeSig(string hourIndex, string minIndex)

    id: timeSettingArea
    width: 190
    height: 160

    Rectangle {
        id: timeSettingAreaWin
        width: parent.width
        height: 35
        MouseArea {
            anchors.fill: parent
        }
        Text {
            id: timeSettingAreaWinText
            text: title
            font.pointSize: 16
            anchors.centerIn: parent
        }
    }

    Rectangle {
        width: parent.width
        height: 1
        color: "#aeaeae"
        anchors.top: timeSettingAreaWin.bottom
        z: 1.5
    }

    Rectangle {
        id: hourTimeArea
        width: parent.width/2
        height: parent.height-timeSettingAreaWin.height
        anchors.top: timeSettingAreaWin.bottom
        anchors.left: parent.left
        color: "#c0c2cc"

        PathView {
            id: pathViewHourTime
            width: timeSettingArea.width/2
            height: timeSettingArea.height-timeSettingAreaWin.height
            pathItemCount: 3
            preferredHighlightBegin: 0.5
            preferredHighlightEnd: 0.5
            highlight: Rectangle {color: "#5cbafa"; width: parent.width; height: parent.height/3-10}
            dragMargin: pathViewHourTime.width/2
            delegate: Rectangle {
                id: viewDelegate
                width: pathViewHourTime.width
                height: pathViewHourTime.height/3
                color: "#00000000"
                Text {
                    anchors.centerIn: parent;
                    font.pixelSize: 21;
                    text: stitchingNumbers(index);
                    color: viewDelegate.PathView.isCurrentItem?"white": "black"
                }
            }
            highlightMoveDuration: 50
            model: 24
            clip: true
            path: Path {
                startX: pathViewHourTime.width/2; startY: 10// -itemHeight/2
                PathLine { x: pathViewHourTime.width/2; y: pathViewHourTime.pathItemCount*20 + 25+30 }
            }
        }
    }

    Rectangle {
        id: lineId
        z: 1.5
        width:1
        height: pathViewHourTime.height-10
        anchors.verticalCenter: minutesArea.verticalCenter
        anchors.left: minutesArea.left
        color: "gray"

    }

    Rectangle {
        id: minutesArea
        width: parent.width/2
        height: parent.height-timeSettingAreaWin.height
        anchors.top: timeSettingAreaWin.bottom
        anchors.left: hourTimeArea.right
        color: "#c0c2cc"
        PathView {
            id: pathViewMinTime
            width: timeSettingArea.width/2
            height: timeSettingArea.height-timeSettingAreaWin.height
            pathItemCount: 3
            preferredHighlightBegin: 0.5
            preferredHighlightEnd: 0.5
            highlight: Rectangle {color: "#5cbafa"; width: parent.width; height: parent.height/3-10}
            dragMargin: pathViewHourTime.width/2
            delegate: Rectangle {
                id: viewDelegate2
                width: pathViewHourTime.width
                height: pathViewHourTime.height/3
                color: "#00000000"
                Text {
                    anchors.centerIn: parent;
                    font.pixelSize: 21;
                    text: stitchingNumbers(index);
                    color: viewDelegate2.PathView.isCurrentItem?"white": "black"
                }
            }
            highlightMoveDuration: 50
            model: 60
            clip: true
            path: Path {
                startX: pathViewHourTime.width/2; startY: 10
                PathLine { x: pathViewHourTime.width/2; y: pathViewHourTime.pathItemCount*20 + 25+30 }
            }
        }
    }

    onCurrentIndexHourChanged: {
        timeSig(stitchingNumbers(currentIndexHour), stitchingNumbers(currentIndexMin))
    }

    onCurrentIndexMinChanged: {
        timeSig(stitchingNumbers(currentIndexHour), stitchingNumbers(currentIndexMin))
    }

    function stitchingNumbers(index) {
        if(index < 10)
            return "0"+index
        return index
    }
}
