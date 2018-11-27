#include "functionui.h"

FunctionUi::FunctionUi(QObject *parent) :
    UiBase(parent)
{
}

FunctionUi::FunctionUi(ThemeParser *pthemeManager, QDomElement &pElement)
{

}

bool FunctionUi::initGeometry(DIRECTION direction, int layout){
    QVariant arg1 = getComRect(direction,layout);
    return invokeQmlMethod("setGeometry",arg1);
}

bool FunctionUi::initUi(DIRECTION direction)
{
    Q_UNUSED(direction);
}

bool FunctionUi::setFunction(int index){
    QString path = getResQmlPath(index);
    //IDE_TRACE_STR(path);
    if(!path.isEmpty()){
        if(QString::compare(functionPath,path,Qt::CaseSensitive)){
            functionPath = path;
            setFunction(path);
        }
    }

}

QString FunctionUi::tipText(int index){
    RcItem *tmpRcInfo = m_ComponentRCs.value(index);
    if(tmpRcInfo){
        return  tmpRcInfo->mTipText;
    }
    return "";
}

bool FunctionUi::setFunction(QString logoPath){
    QVariant arg1 = logoPath;
    return invokeQmlMethod("setFunction",arg1);
}


