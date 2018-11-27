/********************************************************************************
** Form generated from reading UI file 'fileupdate.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILEUPDATE_H
#define UI_FILEUPDATE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FileUpdate
{
public:
    QProgressBar *progressBar;
    QLabel *label_text;

    void setupUi(QWidget *FileUpdate)
    {
        if (FileUpdate->objectName().isEmpty())
            FileUpdate->setObjectName(QStringLiteral("FileUpdate"));
        FileUpdate->resize(1024, 768);
        QFont font;
        font.setFamily(QStringLiteral("FZCuQian-M17S"));
        FileUpdate->setFont(font);
        progressBar = new QProgressBar(FileUpdate);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setGeometry(QRect(460, 471, 118, 23));
        progressBar->setValue(0);
        label_text = new QLabel(FileUpdate);
        label_text->setObjectName(QStringLiteral("label_text"));
        label_text->setGeometry(QRect(340, 410, 381, 31));
        label_text->setFont(font);

        retranslateUi(FileUpdate);

        QMetaObject::connectSlotsByName(FileUpdate);
    } // setupUi

    void retranslateUi(QWidget *FileUpdate)
    {
        FileUpdate->setWindowTitle(QApplication::translate("FileUpdate", "Form", 0));
        label_text->setText(QApplication::translate("FileUpdate", "system is updating , please wait patiently\342\200\246\342\200\246", 0));
    } // retranslateUi

};

namespace Ui {
    class FileUpdate: public Ui_FileUpdate {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILEUPDATE_H
