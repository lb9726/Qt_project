#include "templeteload.h"
#include <QApplication>
#include "./util/filefunc.h"
TempleteLoad::TempleteLoad(QObject *parent) : QObject(parent)
{
    m_editor = (UiEditor*)parent;
    m_templeteDir =  QApplication::applicationDirPath();
    m_templeteDir.replace("\\", "/");
    if(!m_templeteDir.endsWith('/'))
        m_templeteDir.append('/');
    m_templeteDir.append("templete/");
    scanTemplete();
}

void TempleteLoad::scanTemplete()
{
    m_templeteList.clear();
    QDir tmpDir(m_templeteDir);
    if(!tmpDir.exists()){
        m_templeteList.clear();
        return ;
    }

    QStringList folders = tmpDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (int i = 0; i < folders.size(); ++i)
    {
        if(QFile::exists(m_templeteDir+folders[i]+"/Theme.ui")){
            m_templeteList<<folders[i];
        }
    }
}

QStringList TempleteLoad::getTemplete()
{
    scanTemplete();
    return m_templeteList;
}

bool TempleteLoad::createTemplete(QString path,QString proName,int tempIndex)
{
    bool flag = false;
    if(tempIndex >= m_templeteList.count() || tempIndex <0){
        IDE_TRACE();
        return false;
    }
    path.replace("\\", "/");
    if(!path.endsWith('/')){
        path.append('/');
    }
    path.append(proName+"/");
    DeleteDirectory(path);
    flag = CreatPath(path);
    if(!flag){
        IDE_TRACE();
        return false;
    }
    flag = CopyDirectory(m_templeteDir+ m_templeteList.at(tempIndex),path,true);
    if(!flag){
        return false;
    }
    if( m_editor == NULL){
        return false;
    }

    return  m_editor->loadUiFile(path+"Theme.ui");
}
