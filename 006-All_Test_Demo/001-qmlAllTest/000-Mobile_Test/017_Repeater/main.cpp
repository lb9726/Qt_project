#include <QGuiApplication>
#include <QQuickView>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQuickView quickView;
    quickView.setSource(QStringLiteral("qrc:/main.qml"));
    quickView.show();

    return app.exec();
}
