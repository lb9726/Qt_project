import QtQuick.Controls.Material 2.0
import QtQuick 2.4
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

ApplicationWindow {
    visible: true
    property int dpi: 1
    property string name:"姓名"
    width : 300 *dpi
    height: 300 * dpi

    ListModel {
        id: listModel
        ListElement { name: "Apple555555555555555555555555555555555555" }
        ListElement { name: "Orange555555555555555555555555555555555555" }
        ListElement { name: "Banana555555555555555555555555555555555555" }
        ListElement {name: "Computer1555555555555555555555555555555555555"}
        ListElement {name: "Computer2555555555555555555555555555555555555"}
        ListElement {name: "Computer3555555555555555555555555555555555555"}
        ListElement {name: "Computer4555555555555555555555555555555555555"}
        ListElement {name: "Computer5555555555555555555555555555555555555"}
        ListElement {name: "Computer6555555555555555555555555555555555555"}
        ListElement {name: "Computer7555555555555555555555555555555555555"}
        ListElement {name: "Computer8555555555555555555555555555555555555"}
    }

    ScrollView {
        anchors.fill: parent
        ListView {
            id:left_list

            model:listModel

            clip:true
            orientation:  ListView.horizontalCenter
            snapMode   :  ListView.SnapToItem       //停靠在列表的最开始
            cacheBuffer:  20
            anchors.fill: parent

            delegate: Rectangle{
                id:delegate_list
                // color: "red"
                height: 40*dpi
                width: 300*dpi
                signal signalShowMenu(var id,int x,int y)

                //高亮
                MouseArea{
                    id:mouse_delegate

                    acceptedButtons: Qt.RightButton|Qt.LeftButton
                    hoverEnabled: true
                    propagateComposedEvents: true
                    enabled:true
                    anchors.fill: parent
                    onEntered:{
                        delegate_list.color = "#DCDCDC"
                        btn_del.visible     = true
                        //  console.log("in")
                    }
                    onExited:{
                        delegate_list.color = "white"
                        btn_del.visible     = false
                        //  console.log("out")
                    }
                    onClicked: {
                        mouse.accepted = false;
                        console.log("item click.");
                    }
                    onDoubleClicked: {
                        mouse.accepted = false;
                        console.log("item double click.");
                    }
                }
                RowLayout{

                    spacing: 20 *dpi

                    anchors{
                        left: parent.left
                        leftMargin: 12*dpi
                        right: parent.right
                        rightMargin: 12*dpi
                    }
                    //用于将该行显示在矩形垂直居中
                    anchors.verticalCenter: parent.verticalCenter

                    Label{
                        id:left_list_user

                        anchors {
                            leftMargin: 12*dpi
                            left: left_list_photo.right
                        }

                        text:name
                        color:"#333333"
                        font.pixelSize: 15 * dpi
                        font.family: qsTr("微软雅黑")
                    }


                    //删除按钮
                    Button{
                        id:btn_del

                        anchors.right: parent.right
                        anchors.rightMargin: 15*dpi
                        iconSource:"./image/del.png"
                        width   : 20*dpi
                        height  : width
                        visible:false
                        enabled:true

                        style  : ButtonStyle{
                            background:Image {
                                width: sourceSize.width * dpi
                                height: sourceSize.height * dpi
                                source:control.iconSource
                            }
                            label:Item {}
                        }
                        onHoveredChanged: {
                            hovered ? delegate_list.color = "#DCDCDC" : delegate_list.color = "white"
                            hovered ? btn_del.visible = true : btn_del.visible = true
                        }

                        onClicked: {
                            console.log("del item")
                        }
                    }
                }
            }
        }
    }
}
