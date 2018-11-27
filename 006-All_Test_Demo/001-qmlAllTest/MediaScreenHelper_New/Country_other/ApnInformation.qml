import QtQuick 2.7
import QtQuick.Layouts 1.0
import QtQuick.Controls.Styles 1.3
import QtQuick.Controls 1.3

Item {
    property alias newwidth: idItem.width
    property alias newHeight: idItem.height
    property var yysList: ["移动","联通"]
    property var jrdList: ["aaa", "bbb"]
    property bool yysTexteditFlag: false

    id: idItem
    width: 640
    height: 60

    ColumnLayout {
        RowLayout {
            ColumnLayout {
                Item {
                    width: countryId.width
                    height: 15
                    Label {
                        id: countryTextId
                        text:qsTr("国家")
                        anchors.centerIn: parent
                    }
                }
                ComboBox {
                    id: countryId
                    model: ["china","Singapore","Others"]
                    width: 80
                    onCurrentIndexChanged: {
                        yysList = MediaScreen.getModel(currentIndex)
                        if (currentIndex > 1)
                        {
                            yysTexteditFlag = true
                        }
                        else
                        {
                            yysTexteditFlag = false
                        }
                    }
                }
            }
            ColumnLayout {
                Item {
                    width: yunyinshangId.width
                    height: 10
                    Item {
                        width: yunyinshangId.width
                        y: -10
                        Label {
                            id: yysTextId
                            text: qsTr("运营商")
                            anchors.centerIn: parent
                        }
                    }
                }
                Item {
                    ComboBox {
                        id: yunyinshangId
                        y: 2
                        model: yysList
                        width: 80
                    }
                    TextField {
                        id: yysTextInputId
                        y: 2
                        visible: yysTexteditFlag
                        placeholderText: qsTr("编辑...")
                        horizontalAlignment: TextInput.AlignHCenter
                        style: TextFieldStyle { // 自定义样式
                            background: Rectangle {
                                radius: 3
                                implicitWidth: 80
                                implicitHeight: 25
                                border.width: 1
                                border.color: "black"
                            }
                        }
                    }
                }
            }
            ColumnLayout {
                Item {
                    width: jrdTextInputId.width
                    height: 15
                    Label {
                        id: jrdTextId
                        text:qsTr("接入点")
                        anchors.centerIn: parent
                    }
                }

                TextField {
                    id: jrdTextInputId
                    horizontalAlignment: TextInput.AlignHCenter
                    placeholderText: qsTr("编辑...")
                    style: TextFieldStyle { // 自定义样式
                        background: Rectangle {
                            radius: 3
                            implicitWidth: 80
                            implicitHeight: 25
                            border.width: 1
                            border.color: "black"
                        }
                    }
                }
            }
            ColumnLayout {
                Item {
                    width: mccTextInputId.width
                    height: 15
                    Label {
                        id: mccTextId
                        text:qsTr("mcc")
                        anchors.centerIn: parent
                    }
                }
                TextField {
                    id: mccTextInputId
                    horizontalAlignment: TextInput.AlignHCenter
                    placeholderText: qsTr("编辑...")
                    style: TextFieldStyle { // 自定义样式
                        background: Rectangle {
                            radius: 3
                            implicitWidth: 80
                            implicitHeight: 25
                            border.width: 1
                            border.color: "black"
                        }
                    }
                }
            }
            ColumnLayout {
                Item {
                    width: mncTextInputId.width
                    height: 15
                    Label {
                        id: mncTextId
                        text:qsTr("mnc")
                        anchors.centerIn: parent
                    }
                }
                TextField {
                    id: mncTextInputId
                    horizontalAlignment: TextInput.AlignHCenter
                    placeholderText: qsTr("编辑...")
                    style: TextFieldStyle { // 自定义样式
                        background: Rectangle {
                            radius: 3
                            implicitWidth: 80
                            implicitHeight: 25
                            border.width: 1
                            border.color: "black"
                        }
                    }
                }
            }
            ColumnLayout {
                Item {
                    width: userTextInputId.width
                    height: 15
                    Label {
                        id: userTextId
                        text:qsTr("用户名")
                        anchors.centerIn: parent
                        // horizontalAlignment: Text.horizontalAlignment
                    }
                }
                TextField {
                    id: userTextInputId
                    horizontalAlignment: TextInput.AlignHCenter
                    placeholderText: qsTr("编辑...")
                    style: TextFieldStyle { // 自定义样式
                        background: Rectangle {
                            radius: 3
                            implicitWidth: 80
                            implicitHeight: 25
                            border.width: 1
                            border.color: "black"
                        }
                    }
                }
            }
            ColumnLayout {
                Item {
                    width: pwdTextInputId.width
                    height: 15
                    Label {
                        id: passwdTextId
                        text:qsTr("密码")
                        anchors.centerIn: parent
                    }
                }
                TextField {
                    id: pwdTextInputId
                    horizontalAlignment: TextInput.AlignHCenter
                    placeholderText: qsTr("编辑...")
                    echoMode: TextInput.Password
                    style: TextFieldStyle { // 自定义样式
                        background: Rectangle {
                            radius: 3
                            implicitWidth: 80
                            implicitHeight: 25
                            border.width: 1
                            border.color: "black"
                        }
                    }
                }
            }
        }
    }

    function translateText()
    {
        countryTextId.text = qsTr("国家")
        yysTextId.text = qsTr("运营商")
        jrdTextId.text = qsTr("接入点")
        userTextId.text = qsTr("用户名")
        passwdTextId.text = qsTr("密码")
        yysTextInputId.placeholderText = qsTr("编辑...")
        jrdTextInputId.placeholderText = qsTr("编辑...")
        mccTextInputId.placeholderText = qsTr("编辑...")
        mncTextInputId.placeholderText = qsTr("编辑...")
        userTextInputId.placeholderText  = qsTr("编辑...")
        pwdTextInputId.placeholderText = qsTr("编辑...")
    }

    function getAPNInformation()
    {
        var countryIndex = countryId.currentIndex
        var yysIndex = yunyinshangId.currentIndex
        var yystext = ""

        var jrd = jrdTextInputId.text;
        var mcc = mccTextInputId.text;
        var mnc = mncTextInputId.text;
        var username = userTextInputId.text;
        var passwd = pwdTextInputId.text;
        if (true === yysTexteditFlag) // 如果country选择了others
        {
            yystext = yysTextInputId.text;
            ConfigureSerialer.setCountryIndex(countryIndex, -1) // 给yysIndex设置为-1
        }
        else
        {
            ConfigureSerialer.setCountryIndex(countryIndex, yysIndex)
        }
        ConfigureSerialer.setApnParameter(yystext, jrd, mcc, mnc, username, passwd)
    }
}
