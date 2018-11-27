import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.2

Item {
    signal sig_local();
    signal sig_online();
    signal sig_quit();

    RowLayout {
        width: windowWidth/2+100
        height: windowWidth/2
        anchors.centerIn: parent
        spacing: windowWidth/10
        ColumnLayout {
            spacing: 50
            Image {
                source: "qrc: /images/online_ui.png"
                //Layout.fillWidth: true
                Layout.minimumWidth: 100
                Layout.minimumHeight: 100
                Layout.maximumWidth: 300
                Layout.maximumHeight: 300
                Layout.preferredWidth: 120
                Layout.preferredHeight: 140
                Layout.alignment: Qt.AlignHCenter

            }

            Rectangle {
                color: viceColor
                Layout.fillWidth: true
                Layout.minimumWidth: 100
                Layout.preferredWidth: 250
                Layout.preferredHeight: 60
                Text {
                    text: "在线制作"
                    anchors.centerIn: parent
                    color: "white"
                    font.pixelSize: parent.width/10
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: uiLogicalEngine(1,1);
                    onPressed: parent.opacity = themeOpacity;
                    onReleased: parent.opacity = 1;
                }
            }
        }

        ColumnLayout {
            spacing: 50
            Image {
                source: "qrc: /images/local_ui.png"
                Layout.minimumWidth: 100
                Layout.minimumHeight: 100

                Layout.maximumWidth: 300
                Layout.maximumHeight: 300

                Layout.preferredWidth: 120
                Layout.preferredHeight: 140
                Layout.alignment: Qt.AlignHCenter
            }
            Rectangle {
                color: viceColor
                Layout.fillWidth: true
                Layout.minimumWidth: 100
                Layout.preferredWidth: 250
                Layout.preferredHeight: 60
                Text {
                    text: "本地制作"
                    anchors.centerIn: parent
                    color: "white"
                    font.pixelSize: parent.width/10
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: uiLogicalEngine(1,2);
                    onPressed: parent.opacity = themeOpacity;
                    onReleased: parent.opacity = 1;
                }
            }
        }
    }

    Button {
        width: windowWidth/20 > 50 ? 50: windowWidth/20
        height: windowWidth/20 > 50 ? 50: windowWidth/20

        anchors {
            left: parent.left
            top: parent.top
            margins: windowWidth > windowHeight  ? windowHeight/20 : windowWidth/20
        }

        style: ButtonStyle {
            background: Image {
                source: "qrc: /images/link_status.png"
            }
        }
        opacity: !enabled ? 0.4: (pressed ? themeOpacity : 1)
        enabled: false
        onClicked: {
            //uiLogicalEngine(10)
        }
    }

    Button {
        id: bt_quit
        width: windowWidth/20 > 50 ? 50: windowWidth/20
        height: windowWidth/20 > 50 ? 50: windowWidth/20

        opacity: pressed ? themeOpacity : 1

        anchors {
            right: parent.right
            top: parent.top
            margins: windowWidth > windowHeight  ? windowHeight/20 : windowWidth/20
        }

        style: ButtonStyle {
            background: Image {
                source: "qrc: /images/quit_button.png"
            }
        }

        onClicked: {
            uiLogicalEngine(1,0)
        }
    }
}
