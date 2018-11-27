#ifndef MESSAGEBAR_H
#define MESSAGEBAR_H

#include <QWidget>
#include <QLabel>
namespace Ui {
class MessageBar;
}

class MessageBar : public QWidget
{
    Q_OBJECT

public:
    explicit MessageBar(QWidget *parent = 0);
    ~MessageBar();

private:
    Ui::MessageBar *ui;
public:
    QLabel *m_messageLabel;
};

#endif // MESSAGEBAR_H
