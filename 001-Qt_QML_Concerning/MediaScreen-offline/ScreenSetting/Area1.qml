import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Item {
    ColumnLayout {
        spacing: 10
        RowLayout{
            CheckBox {
                id: lightandvolume
                text: qsTr("亮度&音量")
            }
        }
        RowLayout {
            spacing: 10
            Label {
                text: qsTr("亮度")
            }
            Slider {
                width: 50
                from:0
                to:100
                value: 0
            }
            SpinBox {
                from: 0
                to: 100
                value: 0
            }
        }
        RowLayout {
            spacing: 10
            Label {
                text: qsTr("音量")
            }
            Slider {
                width: 50
                from:0
                to:100
                value: 0
            }
            SpinBox {
                width: 50
                from: 0
                to: 100
                value: 0
            }
        }
        RowLayout {
            Label {
                text: qsTr("滚动文字")
            }
        }
        RowLayout {
            CheckBox {
                id: showscrolltext
                text: qsTr("隐藏滚动文字")
                onCheckedChanged: {
                    if (checked)
                    {
                        showscrolltext.text = qsTr("显示滚动文字")
                    }
                    else
                    {
                       showscrolltext.text = qsTr("隐藏滚动文字")
                    }
                }
            }
        }
        RowLayout {
//            Item {
//                width: 30
//            }
            TextField {
                id: scrolltext
//                width: 300
                placeholderText: qsTr("请输入字幕文字")
            }
        }
        RowLayout {
            Label {
                id: title_id
                text: qsTr("标题")
            }
        }
        RowLayout {
            CheckBox {
                id: showtitle_id
                text: qsTr("隐藏标题")
                onCheckedChanged: {
                    if (checked)  // 多国语言翻译的时候需要修改
                    {
                        showtitle_id.text = qsTr("显示标题")
                    }
                    else
                    {
                        showtitle_id.text = qsTr("隐藏标题")
                    }
                }
            }
        }
        RowLayout {
            Item {
                width: 40
            }
            TextField {
                id: titletext
                placeholderText: qsTr("请输入标题文字")
                anchors.centerIn: parent
            }
        }
    }

}
