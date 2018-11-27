#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_quickWidget(new QQuickWidget)
{
    initWidget();
}

MainWindow::~MainWindow()
{

}

void MainWindow::initWidget()
{
    QMdiArea *centralWidget = new QMdiArea;

    QUrl source("qrc:qml/MutiMedia.qml");

    connect(m_quickWidget, &QQuickWidget::statusChanged,
            this, &MainWindow::quickWidgetStatusChanged);
    connect(m_quickWidget, &QQuickWidget::sceneGraphError,
            this, &MainWindow::sceneGraphError);
    m_quickWidget->resize(300, 300);
    m_quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView );
    m_quickWidget->setSource(source);
    centralWidget->addSubWindow(m_quickWidget);

    setCentralWidget(centralWidget);
}

void MainWindow::quickWidgetStatusChanged(QQuickWidget::Status status)
{
    if (status == QQuickWidget::Error) {
        QStringList errors;
        foreach (const QQmlError &error, m_quickWidget->errors())
            errors.append(error.toString());
        statusBar()->showMessage(errors.join(QStringLiteral(", ")));
    }
}

void MainWindow::sceneGraphError(QQuickWindow::SceneGraphError, const QString &message)
{
     statusBar()->showMessage(message);
}
