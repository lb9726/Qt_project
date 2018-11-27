import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Item {
    property int area1_width: 432
    property int area2_width: 650
    property int area3_width: 310
    property int area1_height: 510
    property int area4_height: 144
    ColumnLayout {
        RowLayout {
            Grid {
                rows: 1
                columns: 3
                spacing: 5
                Rectangle {
                    width: area1_width
                    height: area1_height
                    color: "transparent"
                    border.color: "Grey"
                    border.width: 1
                    TestArea1 {
                        x: 8
                        y: 8
                    }
                }
                Rectangle {
                    width: area2_width
                    height: area1_height
                    color: "transparent"
                    border.color: "Grey"
                    border.width: 1
                    TestArea2 {
                        x: 8
                        y: 8
                    }
                }
                Rectangle {
                    width: area3_width
                    height: area1_height
                    color: "transparent"
                    border.color: "Grey"
                    border.width: 1
                    TestArea3 {
                        width: parent.width
                        height: parent.height
                        x: 8
                        y: 8
//                        clip: true
                    }
                }
            }
        }

        RowLayout {
            Grid {
                rows: 1
                columns: 2
                spacing: 5
                Rectangle {
                    width: area1_width
                    height: area4_height
                    color: "transparent"
                    border.color: "Grey"
                    border.width: 1
                   TestArea4 {
                        x: 8
                        y: 8
                    }
                }
                Rectangle {
                    width: area2_width + area3_width + 5
                    height: area4_height
                    color: "transparent"
                    border.color: "Grey"
                    border.width: 1
                    TestArea5 {
                        x: 8
                        y: 8
                        width: parent.width
                        height: parent.height
                    }
                }
            }
        }
    }
}
