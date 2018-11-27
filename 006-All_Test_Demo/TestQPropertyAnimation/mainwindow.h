#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QPropertyAnimation>
#include <QLabel>
#include <QWidget>
#include <QString>

class MainWindow : public QWidget
{
    Q_OBJECT
    // 声明属性
    Q_PROPERTY(int pixmap READ pixmap WRITE setPixmap)

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    // 开始动画
    void startAnimation();
    // 停止动画
    void stopAnimation();

private:
    int pixmap() const;
    // 切换图标
    void setPixmap(const int index);

private:
    QLabel *m_pLoadingLabel;
    QPropertyAnimation *m_pAnimation;
    int m_nIndex;
    QString mRunPath;
};
#endif // MAINWINDOW_H
