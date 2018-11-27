#include "qlabelselfdefine.h"

QlabelSelfDefine::QlabelSelfDefine(QLabel *parent) : QLabel(parent)
{

}

void QlabelSelfDefine::setMyStyleSheet()
{
    setAlignment(Qt::AlignLeft);
    setStyleSheet("QLabel{border:2px solid rgb(85, 170, 255); border-radius:20px}");
}
