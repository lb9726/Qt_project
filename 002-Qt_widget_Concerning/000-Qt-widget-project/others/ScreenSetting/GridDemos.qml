import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
Item {
    ColumnLayout {
        RowLayout {
            Grid {
                rows:1
                columns: 3
                spacing: 5
                Rectangle {
                    width: 389
                    height: 360
                    color: "transparent"
                    border.color: "Grey"
                    border.width: 1
                    Area1 {
                        x: 2
                        y: 2
                    }
                }
                Rectangle {
                    width: 385
                    height: 360
                    color: "transparent"
                    border.color: "Grey"
                    border.width: 1
                    Area2 {
                        x: 2
                        y: 2
                    }
                }
                Rectangle {
                    width: /*173*/ 300
                    height: 360
                    color: "transparent"
                    border.color: "Grey"
                    border.width: 1
//                    Area3 {
//                        width: parent.width
//                        height: parent.height
//                        x: 2
//                        y: 2
//                        clip: true
//                    }
                    SlefDefineArea3 {
                        x: 2
                        y: 2
                    }
                }
            }
        }

        RowLayout {
            Grid {
                rows:1
                columns: 2
                spacing: 5
                Rectangle {
                    width: 389
                    height: 144
                    color: "transparent"
                    border.color: "Grey"
                    border.width: 1
                    Area4 {
                        x: 2
                        y: 2
                    }
                }
                Rectangle {
                    width: 563
                    height: 144
                    color: "transparent"
                    border.color: "Grey"
                    border.width: 1
                    Area5 {
                        x: 2
                        y: 2
                        width: parent.width
                        height: parent.height
                    }
                }
            }
        }
    }

}
