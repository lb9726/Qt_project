/********************************************************************************
** Form generated from reading UI file 'introduce.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INTRODUCE_H
#define UI_INTRODUCE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTextBrowser>

QT_BEGIN_NAMESPACE

class Ui_introduce
{
public:
    QTextBrowser *textBrowser;

    void setupUi(QDialog *introduce)
    {
        if (introduce->objectName().isEmpty())
            introduce->setObjectName(QStringLiteral("introduce"));
        introduce->resize(400, 387);
        textBrowser = new QTextBrowser(introduce);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        textBrowser->setGeometry(QRect(40, 20, 331, 331));
        textBrowser->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));

        retranslateUi(introduce);

        QMetaObject::connectSlotsByName(introduce);
    } // setupUi

    void retranslateUi(QDialog *introduce)
    {
        introduce->setWindowTitle(QApplication::translate("introduce", "Dialog", 0));
        textBrowser->setHtml(QApplication::translate("introduce", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'\303\246\302\226\302\271\303\246\302\255\302\243\303\245\302\260\302\217\303\246\302\240\302\207\303\245\302\256\302\213_GBK'; font-size:12pt; font-weight:600; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt; font-style:italic; color:#f04d4d;\">&lt;&lt;\346\211\253\351\233\267&gt;&gt;\346\230\257\344\270\200\346\254\276\345\244\247\344\274\227\347\261\273\347\232\204\347\233\212\346\231\272\345\260\217\346\270\270\346\210\217,\347\254\254\344\270\200\346\254\276\346\211\253\351\233\267\346\270\270\346\210\217\344\272\2161992\345\271\264\345\217\221\350\241\214 </span></p>\n"
"<p align=\"center\" style=\" margin-top:0px; ma"
                        "rgin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt; font-style:italic; color:#f04d4d;\">\346\270\270\346\210\217\347\233\256\346\240\207\346\230\257\345\234\250\346\234\200\347\237\255\347\232\204\346\227\266\351\227\264\345\206\205\346\240\271\346\215\256\347\202\271\345\207\273\346\240\274\345\255\220</span></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt; font-style:italic; color:#f04d4d;\">\345\207\272\347\216\260\347\232\204\346\225\260\345\255\227\346\211\276\345\207\272\346\211\200\346\234\211\351\235\236\351\233\267\346\240\274\345\255\220,\345\220\214\346\227\266\351\201\277\345\205\215\350\270\251\351\233\267</span></p>\n"
"<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:14pt; font-sty"
                        "le:italic; color:#f04d4d;\"><br /></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt; font-style:italic; color:#f04d4d;\">\346\270\270\346\210\217\345\270\256\345\212\251:\347\202\271\345\207\273\345\267\246\344\276\247\347\232\204&quot;\351\232\276\345\272\246\351\200\211\346\213\251\351\200\211\351\241\271&quot; \345\234\250\344\270\213\346\213\211\346\241\206\344\270\255\345\217\257\344\273\245\350\207\252\347\224\261\351\200\211\346\213\2513\347\247\215\346\270\270\346\210\217\346\250\241\345\274\217,9*9 10\344\270\252\351\233\267,16*16 40\344\270\252\351\233\267\345\222\214\350\207\252\345\256\232\344\271\211\346\250\241\345\274\217</span></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt; font-style:italic; color:#f04d4d;\">\351\274\240\346\240\207\345\267\246"
                        "\351\224\256:\347\277\273\347\234\213  \345\217\263\351\224\256:\346\240\207\350\256\260\345\234\260\351\233\267</span></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt; font-style:italic; color:#f04d4d;\">\345\277\253\346\235\245\350\277\233\345\205\245\346\270\270\346\210\217\345\220\247!!</span></p></body></html>", 0));
    } // retranslateUi

};

namespace Ui {
    class introduce: public Ui_introduce {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INTRODUCE_H
