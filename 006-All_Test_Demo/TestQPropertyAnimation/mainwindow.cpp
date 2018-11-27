#include "mainwindow.h"
#include <QCoreApplication>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
{
    mRunPath = QCoreApplication::applicationDirPath();
    m_pAnimation = new QPropertyAnimation(this, "pixmap");
    m_pLoadingLabel = new QLabel(this);
    m_pLoadingLabel->show();
    m_pAnimation->setDuration(2000);
    // 永远运行，直到stop
    m_pAnimation->setLoopCount(-1);

    // 设置动画各区间的值
    for (int i = 0; i < 5; ++i)
    {
        m_pAnimation->setKeyValueAt(i/4.0, i + 1);
    }
    // 开始动画
    startAnimation();
}

MainWindow::~MainWindow()
{

}

void MainWindow::startAnimation()
{
    m_pAnimation->start();
}

void MainWindow::stopAnimation()
{
    m_pAnimation->stop();
}

int MainWindow::pixmap() const
{
    return m_nIndex;
}

void MainWindow::setPixmap(const int index)
{
    qDebug()<<__PRETTY_FUNCTION__<<"index = "<<index;
    m_nIndex = index;
    QString tmp = mRunPath + QString("/Images/load%1.png").arg(index);
    qDebug()<<__PRETTY_FUNCTION__<<"tmp = "<<tmp;
    QPixmap pixmap(tmp);
    m_pLoadingLabel->setPixmap(pixmap);
}
