#ifndef UIVIDEO_H
#define UIVIDEO_H
#include <QWidget>

class UiVideo : public QWidget
{
    Q_OBJECT
public:
    UiVideo(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
};

#endif // UIVIDEO_H
