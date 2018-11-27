import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

Item {
    width: 640
    height: 480

    property alias button3: button3
    property alias button2: button2
    property alias button1: button1

    RowLayout {
        anchors.centerIn: parent

        Button {
            id: button1
            text: qsTr("Press Me 1")
        }

        Button {
            id: button2
            text: qsTr("Press Me 2")
        }

        Button {
            id: button3
            text: qsTr("Press Me 3")
        }
    }
}
/*
RGBGame 类的成员函数 algorithm()、setAlgorithm(),在 QML 代码中使用时,可以使
用${Object}.${method}来访问,例子参照 main.qml。
类 public 成员函数用 Q_INVOKABLE 宏标识时,就可以通过 Qt 元对象系统来调用,但
这个宏必须放在返回类型前面。
函数参数:如果函数参数的类型是 QObject*,在 QML 代码中使用时,函数参数可以
通过对象 id 来传递,也可以通过指向对象的 JavaScript var 类型的值来传递。
函数重载:QML 支持 C++函数重载,通过函数参数列表的不同进行匹配,但不识别多
个同名不同参数的信号,这种情况下,只有最后一个信号可以通过 QML 来访问。
函数返回值:通过 QML 中的 JavaScrpt 表达式访问 C++函数时,函数返回值会自动转
换为 JavaScrpt 类型的值
 */
