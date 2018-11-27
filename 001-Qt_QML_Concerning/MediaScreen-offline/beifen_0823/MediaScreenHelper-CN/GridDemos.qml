import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
Item {
    property int area1_width: 432
    property int area3_width: 250
    property int area1_height: 460
    property int area4_height: 144
    ColumnLayout {
        RowLayout {
            Grid {
                rows:1
                columns: 3
                spacing: 5
                Rectangle {
                    width: area1_width
                    height: area1_height
                    color: "transparent"
                    border.color: "Grey"
                    border.width: 1
                    Area1 {
                        x: 2
                        y: 2
                    }
                }
                Rectangle {
                    width: area1_width
                    height: area1_height
                    color: "transparent"
                    border.color: "Grey"
                    border.width: 1
                    Area2 {
                        x: 2
                        y: 2
                    }
                }
                Rectangle {
                    width: area3_width
                    height: area1_height
                    color: "transparent"
                    border.color: "Grey"
                    border.width: 1
                    Area3 {
                        width: parent.width
                        height: parent.height
                        x: 2
                        y: 2
                        clip: true
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
                    width: area1_width
                    height: area4_height
                    color: "transparent"
                    border.color: "Grey"
                    border.width: 1
                    Area4 {
                        x: 2
                        y: 2
                    }
                }
                Rectangle {
                    width: area1_width + area3_width + 5
                    height: area4_height
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
