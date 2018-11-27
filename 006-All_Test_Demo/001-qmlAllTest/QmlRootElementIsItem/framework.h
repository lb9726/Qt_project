#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include <QObject>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQuickView>
#include <QDebug>
#include <QQmlContext>
#include <QQuickItem>

class FrameWork : public QObject
{
    Q_OBJECT
public:
    void registerMyQmlType();
    static FrameWork* getInstance();
    int initScene(int argc, char *argv[]);
    ~FrameWork();

private:
    explicit FrameWork(QObject *parent = 0);
    static FrameWork* _instance;

signals:
    void sigChangeUi(int index);
public slots:
    void changeUi(int index);

private:
    QQuickItem* mCurrentWindow;
    QQmlContext* mQmlContext;
    QQuickView* mEngine;
    QGuiApplication *mApp;
};

#endif // FRAMEWORK_H
