#ifndef UITEXT_H
#define UITEXT_H
#include <QWidget>

class UiText : public QWidget
{
    Q_OBJECT
public:
    UiText(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
};

#endif // UITEXT_H
