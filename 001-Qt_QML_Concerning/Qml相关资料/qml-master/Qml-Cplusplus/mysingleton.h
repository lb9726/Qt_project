#ifndef MYSINGLETON_H
#define MYSINGLETON_H

#include <QObject>
#include <QQmlEngine>
class MySingleton : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int age READ age WRITE setAge NOTIFY ageChanged)
public:
    MySingleton(QObject *parent = 0):
        QObject(parent),
        m_age(0)
    {

    }

    ~MySingleton() {}
    Q_INVOKABLE int changAge() {
        setAge(100);
        return m_age;
    }
    int age() const { return m_age;}
    void setAge(int newAge) {m_age = newAge; emit ageChanged(newAge);}

signals:
    void ageChanged(int newAge);


private:
    int m_age;
};

static QObject* singletonProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);
    MySingleton *mySingleton = new MySingleton();
    return mySingleton;
}
#endif // MYSINGLETON_H
