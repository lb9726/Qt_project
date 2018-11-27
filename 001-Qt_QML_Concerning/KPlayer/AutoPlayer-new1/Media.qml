import QtQuick 2.0
import QtMultimedia 5.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.0
ApplicationWindow{
    id:kPlayer
    width: Screen.width /*1024*/
    height: Screen.height /*768*/
    visible: true
//    flags: Qt.FramelessWindowHint  //隐藏菜单栏
//    flags: Qt.Window | 0x00800000 | Qt.FramelessWindowHint
    Column{
        Rectangle {
            id: shang
            color:"white"
            width: kPlayer.width
            height: 80
            Row {
                width: parent.width
                height: parent.height
                Rectangle {
                    id: picRect
                    width: logo.width
                    height: logo.height
                    Image {
                        id: logo
                        source: "./Images/OTIS.png"
                    }
                }
                /*空白矩形填充，只进行占位*/
                Rectangle {
                    id: blackRect
                    x: picRect.width
                    width:shang.width - picRect.width - bigRect.width - 4
                    height: parent.height
                }

                Rectangle {
                    id: bigRect
                    width: time.width
                    height: parent.height
                    Rectangle {
                        width: time.width
                        height: parent.height / 2
                        anchors.centerIn: parent
                        color:"aqua"
                        Timer {
                            interval: 1000; running: true; repeat: true
                            onTriggered: time.text = Qt.formatDateTime(new Date(), "yyyy-MM-dd dddd hh:mm:ss") //取得系统日期时间
                        }
                        Text {  // text设置
                            id: time
                            font.bold: true
                            font.pixelSize: 24
                            anchors.centerIn: parent  // 设置文本居中
                            color:"blue"
                        }
                    }
                }
            }
        }

        Rectangle {
            id:screen
            color:"black"
            width:kPlayer.width - bigRect.width
            height: kPlayer.height - 130  //130
            Image{
                id:img
                source: "./Images/KPlayer.png"
                anchors.fill: parent
            }

            MediaPlayer{
                id:player
                //source: "file:///home/devin/Desktop/Repository/Qt_project/Qt_project/AutoPlayer-new/videos/Test2.mp4"
                source: "qrc:/videos/Test2.mp4"
                autoLoad: false
                autoPlay: true    // 当AutoPlay 设置为true的时候，如果视频文件存在，就会直接播放视频
                volume: 0.4
                loops: MediaPlayer.Infinite
            }
            VideoOutput {
                anchors.fill: parent
                source: player
            }
            Column {
                Rectangle {              // 显示电梯层号
                    id: lcdnum
                    color:"grey"
                    x: screen.width
                    y: shang.height
                    width: kPlayer.width - screen.width  //宽度
                    height: (kPlayer.height - 130/*130*/) / 2
                    Text {
                        text: "39"
                        anchors.centerIn: parent
                        font.bold: true
                        font.pixelSize: 70
                    }
                }
                Rectangle {             // 电梯禁止吸烟和荷载人数的image图片
                    Image {
                        id: nosomoking
                        x: screen.width
                        y: lcdnum.height  // y坐标就是Lcdnum的高度
                        width: kPlayer.width - screen.width
                        height: (kPlayer.height - 130 /*130 */) / 2
                        source: "./Images/NoSomoking.png"
                    }
                }
            }
        }
        Rollingtext { }
    }
}

