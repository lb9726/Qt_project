#ifndef RGBGAME_H
#define RGBGAME_H
#include<QApplication>
#include<QColor>
class RGBGame: public QObject
{
    Q_OBJECT
    Q_ENUMS(GenerateAlgorithm)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QColor timeColor READ timeColor)
public:
    RGBGame(QObject *parent = 0);
    ~RGBGame();

    enum GenerateAlgorithm {
        RandomRGB,
        RandomRed,
        RandomGreen,
        RandomBlue,
        LinearIncrease
    };

    QColor color() const;
    void setColor(const QColor &color);
    QColor timeColor()const;

    Q_INVOKABLE GenerateAlgorithm algorithm() const;
    Q_INVOKABLE void setAlgorithm(GenerateAlgorithm algorithm);
public slots:
    void start();
    void stop();
signals:
    void colorChanged(const QColor &color);
    void currentTime(const QString &strTime);

protected:
    void timerEvent(QTimerEvent *e);
private:
    GenerateAlgorithm m_algorithm;
    QColor m_currentColor;
    int m_nColorTimer;
};

#endif // RGBGAME_H

/*
 * RGBGame 类中的 color 属性可以在 QML 代码中进行访问、修改,color 属性值改变时
还可发送一个信号来自动更新 QML 中与其绑定的属性值,timeColor 属性也可以在 QML 代
码中进行访问,但不能修改,也没有信号,例子参照 main.qml。
Q_PROPERTY()宏用来在 QObject 派生类中声明属性,这个属性如同类的数据成员一样,
但它又有一些额外的特性可通过 Qt 元对象系统来访问。

下面是 Q_PROPERTY()宏的原型:
Q_PROPERTY()(type name
(READ getFunction [WRITE setFunction] |
MEMBER memberName [(READ getFunction | WRITE setFunction)])
[RESET resetFunction]
[NOTIFY notifySignal]
[REVISION int]
[DESIGNABLE bool]
[SCRIPTABLE bool]
[STORED bool]
[USER bool]
[CONSTANT]
[FINAL])
属性的 type、name 是必需的,其它是可选项,常用的有 READ、WRITE、NOTIFY。属
性的 type 可以是 QVariant 支持的任何类型,也可以是自定义类型,包括类类型、列表类型、
组属性等。另外,属性的 READ、WRITE、RESET 是可以被继承的,也可以是虚函数,这些
特性并不常用。
READ:读取属性值,如果没有设置 MEMBER 的话,它是必需的。一般情况下,函数是
个 const 函数,返回值类型必须是属性本身的类型或这个类型的 const 引用,没有参数。
WRITE:设置属性值,可选项。函数必须返回 void,有且仅有一个参数,参数类型必
须是属性本身的类型或这个类型的指针或引用。
NOTIFY:与属性关联的可选信号。这个信号必须在类中声明过,当属性值改变时,就
可触发这个信号,可以没有参数,有参数的话只能是一个类型同属性本身类型的参数,用来
记录属性改变后的值。
RGBGame 类定义部分已经完成,有 public 槽、信号、枚举类型、Q_INVOKABLE 宏标
识的成员函数、属性等,这些都可以在 QML 代码中访问。
 */
