#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include <QObject>
#include "qmlscreena.h"
#include "qmlscreenb.h"
#include "qmlscreenc.h"
#include <QQuickView>
#include <QtQml>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QQmlContext>

class FrameWork : public QObject
{
    Q_OBJECT
private:
    explicit FrameWork(QObject *parent = 0);
    static FrameWork* _instance;
public:
    void registerMyQmlType();
    static FrameWork* getInstance();
    int runApplication(int argc, char *argv[]);
    void getObject();
signals:

public slots:
    void changeUi(int index);

private:
    QGuiApplication *mapp;
    QQuickView *mview;
    QQmlContext* mQmlContext;
    QQuickItem* tmpItem;
};

#endif // FRAMEWORK_H
