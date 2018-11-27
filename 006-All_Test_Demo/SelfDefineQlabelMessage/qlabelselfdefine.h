#ifndef QLABESELFDEFINE_H
#define QLABESELFDEFINE_H

#include <QLabel>

class QlabelSelfDefine : public QLabel
{
    Q_OBJECT
public:
    explicit QlabelSelfDefine(QLabel *parent = 0);
    void setMyStyleSheet();

signals:

public slots:
};

#endif // QLABESELFDEFINE_H
