import QtQuick 2.4
import QtQuick.Controls 1.3   // 导入Qt Quick Contorls 1.3库
import QtQuick.Layouts 1.1    // 导入Qt Quick 布局库

Item {                        // QML通用的根元素
    width: 640
    height: 480
/*定义属性别名，为在main.qml中引用各个控件*/
    property alias name: name                 // “姓名”文本框
    property alias age: age                   // “年龄”文本框
    property alias sex: sexGroup              // “性别”文本框
    property alias spec: speCBox              // “专业”组合框
    property alias hobby: hobbyGrid           //  “爱好”GridLayout元素
    property alias submit: submit             //  “提示”按钮
    property alias details: stuInfo           //  “基本信息”文本区

    RowLayout {                                //行布局
        x: 50; y:35
        spacing: 10
        ColumnLayout {                          // 列布局
            spacing: 10
            RowLayout {
                spacing: 0
                Label {
                    text:"姓名"                 // 标签
                }
                TextField {
                    id: name
                    placeholderText: qsTr("请输入…")   // (a)
                    focus: true
                }
            }

            RowLayout {
                spacing: 0
                Label {
                    text: "年龄"
                }
                TextField {
                    id: age
                    validator: IntValidator { bottom: 16; top: 26; } //(b)
                }
            }

            GroupBox {   //  组框
                id: group1
                title: qsTr("性别")
                Layout.fillWidth: true                  // (c)
                RowLayout {
                    ExclusiveGroup { id: sexGroup }    // 互斥选项组
                    RadioButton {
                        text: qsTr("男")
                        checked: true
                        exclusiveGroup: sexGroup           // (d)
                        Layout.minimumWidth: 65            // 设置控件所占最小宽度为65
                    }
                    RadioButton {
                        text: qsTr("女")
                        exclusiveGroup: sexGroup
                        Layout.minimumWidth: 65
                    }
                }
            }

      RowLayout {
          spacing: 0
          Label {
              text: "专业"
          }
          ComboBox {                        // 组合框
              id: speCBox
              Layout.fillWidth: true
              currentIndex: 0               // 初始选中项的索引为0（即“计算机”）
              model: ListModel {            // (e)
                  ListElement { text: "计算机" }
                  ListElement { text: "通信工程" }
                  ListElement { text: "信息网络" }
              }
              width: 200
          }
      }

      GroupBox {
          id: group2
          title: qsTr("爱好")
          Layout.fillWidth: true
          GridLayout {
              id: hobbyGrid
              columns: 3
              CheckBox {                    // 复选框
                  text: qsTr("旅游")
                  checked: true             // 默认选中
              }
              CheckBox {
                  text: qsTr("游泳")
                  checked: true
              }
              CheckBox {
                  text: qsTr("篮球")
              }
              CheckBox {
                  text: qsTr("唱歌")
              }
              CheckBox {
                  text: qsTr("舞蹈")
              }
              CheckBox {
                  text: qsTr("网络")
              }
              CheckBox {
                  text: qsTr("看电视")
                  checked: true
              }
              CheckBox {
                  text: qsTr("其他")
                  checked: true
              }
          }
      }

      Button {                             //  命令按钮
          id: submit
          anchors.right: group2.right      // 与”爱好“组框右边框锚定
          text: "提交"
      }
    }

      ColumnLayout {
          Layout.alignment: Qt.AlignTop     // 使”基本信息“ 文本区与表单的顶端对齐
          Label {
              text: "基本信息"
              font.pixelSize: 15
              font.bold: true
          }
          TextArea {
              id: stuInfo
              Layout.fillHeight: true       // 将文本区拉伸至表单等高
              width: 240
              text: "学生个人资料..."         // 初始文字
              font.pixelSize: 14
          }
      }
    }
}

/*
(a)placeholderText: qsTr("请输入…"):placeholderText 是文本框控件的属性，
它设定当文本框的内容为空时其中所要显示的文本（多为提示性的文字），用于
引导用户输入的。

(b)validator：IntValidtor {button:16;top:26;}: validator属性是在文本框
控件上设一个验证器，只有当用户的输入符合验证要求时才能被文本框接受，目前，
Qt Quick 支持的验证器有IntValidator（整形验证器),DoubleValidatro（双精度
浮点验证器）和RegExpValidator（正则表达式验证器）三种。这里使用的是整形验证器
，限定了文本框只能输入16~26（学生年龄段）之间的整形数值。

（c）Layout.fillwidth：true:在Qt Quick 中另有一套独立于QML的布局系统（Qt Quick
布局），其所用的RowLayout，ColumnLayout和GridLayout 类同于QML的Row，Column和Grid
定位器，所在库是QtQuick.Layout 1.1，但它比传统QML的功能更强大，本例程序就充分使用了
这套全新的布局系统。该系统的Layout元素提供了很多”依附属性“，其作用等同于QML的Anchor（锚）。
这里Layout.fillWidth设为true是”性别“ 组框在允许的约束范围内尽可能宽。此外，Layout还有其他
一些常用的属性，如fillHeight，minimumWidth/maximumWidth，minimHeight/maximumHeight，
alignment等，他们的具体应用请参考Qt5官方文档，此处不展开

（d） exclusiveGrop：sexGroup：指定单选按钮所属的ExclusiveGroup（互斥选项组），凡是位于
一个ExclusiveGroup中的可选控件在同一时刻有且仅有一个能被选中。目前有Action，MenuItem，Button
和RadioButton组件支持ExclusiveGroup，但ExclusiveGroup仅仅支持Action 作为其子元素，故编程时
需要为ExclusiveGroup 中其他类型的对象特别指派exclusiveGroup属性。

（e）model:ListModel {……}： 往组合框下拉列表中添加项有两种方式。第一种是本例采用的为其model
属性指派一个ListModel对象，其每个ListElement子元素代表一个列表项；第二种是直接将一个字符串
列表赋值给model属性。故本例中的代码也可以为：
ComboBox {
    ……
    model: ["计算机","通信工程","信息网络"]
    width: 200
}
*/
