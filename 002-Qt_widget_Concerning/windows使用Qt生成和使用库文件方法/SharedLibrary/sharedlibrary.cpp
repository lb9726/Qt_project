#include "sharedlibrary.h"
#include <QPushButton>
#include <QMessageBox>
#include <QTime>

SharedLibrary::SharedLibrary(QWidget *parent)
    : QWidget(parent)
{
    QPushButton *pButton = new QPushButton(this);
    pButton->setText("Test Shared Library");

    connect(pButton, SIGNAL(clicked()), this, SLOT(onClicked()));
}

void SharedLibrary::onClicked()
{
    QMessageBox::information(this, "Tip", "Shared Library...");
}

void SharedLibrary::updateBackground()
{
    QTime time;
    time = QTime::currentTime();
    qsrand(time.msec() + time.second()*1000);

    int nR = qrand() % 256;
    int nG = qrand() % 256;
    int nB = qrand() % 256;

    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, QColor(nR, nG, nB));
    this->setPalette(palette);
}

int SharedLibrary::subtract(int a, int b)
{
    return a - b;
}

SharedLibrary* getSharedLibrary()
{
    return new SharedLibrary();
}

int add(int a, int b)
{
    return a + b;
}
