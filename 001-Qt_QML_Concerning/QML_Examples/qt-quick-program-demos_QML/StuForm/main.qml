import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

ApplicationWindow {
    title: qsTr("学生信息表单")
    width: 640
    height: 480
    visible: true

    Mainform {
        anchors.fill: parent
        submit.onClicked: {                                  // 单击"提交"按钮所要执行的代码
            var hobbyText = ""                               // 变量用于存放学生兴趣爱好的内容
            var max
            for (var j = 0; j < 7; j++)
            {
                if (hobby.children[j].checked)
                {
                    max = j;  // 找到最大的索引值
                }
            }

            for (var i = 0; i < 7; i++) {                    // 遍历”爱好“组框中的复选框
                /*生成学生兴趣爱好文本*/
                if (hobby.children[7].checked) // 若 ”其他“ 复选框选中
                {
                    hobbyText += hobby.children[i].checked ? (hobby.children[i].text + "、") : "";  // (a)
                }
                else
                {
                    if (i < max)
                    {
                        hobbyText += hobby.children[i].checked ? (hobby.children[i].text + "、") : "";  // (a)
                    }
                    else
                    {
                        hobbyText += hobby.children[i].checked ? (hobby.children[i].text) : "";  // (a)
                    }
                }

            }
            if (hobby.children[7].checked) // 若 ”其他“ 复选框选中
            {
                hobbyText += "..."
            }

            /* 最终生成的完整学生信息*/
            details.text = "我的名字叫" + name.text + ",是个" + age.text + "岁的" + sex.current.text +
                    "生，所学专业是" + spec.currentText + ",业余喜欢" + hobbyText;
        }
    }
}
