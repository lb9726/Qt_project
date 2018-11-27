#ifndef AUDIOEDITOR_H
#define AUDIOEDITOR_H

#include "tabpage.h"
#include "waveform.h"
#include <QVector>
#include <QVariant>
#include <QWidget>

typedef enum{
    Wait,
    Busy,
    Err,
    Finished
}Tree_Member_State;

typedef struct{
    QString                     mPath;
    QString                     mName;
    quint32                     mSize;
    quint32                     mLenth;
    quint32                     mSpectrumCnt;
}Audio_Info;

typedef struct{
    Audio_Info                  mAudioInfo;
    Tree_Member_State           mState;
    quint8                      mProgress;
}Tree_Member_Info;

namespace Ui {
class AudioEditor;
}

class MemberInfo
{
public:
    MemberInfo()
    {

    }
    ~MemberInfo()
    {

    }
    int count()
    {
        return m_Tree_Members.count();
    }
    Tree_Member_Info at(int pdex)
    {
        return m_Tree_Members.at(pdex);
    }
    void append(Tree_Member_Info pMember)
    {
        m_Tree_Members.append(pMember);
    }
    bool removeAt(int pDex)
    {
        if((pDex < 0)&&(pDex>= count() - 1))
            return false;
            m_Tree_Members.removeAt(pDex);
        return true;
    }
    void clear()
    {
        m_Tree_Members.clear();
    }
    bool replace(int pDex, Tree_Member_Info pMember)
    {
        if((pDex < 0)&&(pDex>= count() - 1))
            return false;
        m_Tree_Members.replace(pDex, pMember);
        return true;
    }
    bool move(int pSrcDex, int pDstDex)
    {
        if((pSrcDex < 0)&&(pSrcDex>= count() - 1))
            return false;
        if((pDstDex < 0)&&(pDstDex>= count() - 1))
            return false;
        if(pSrcDex == pDstDex)
            return true;
        m_Tree_Members.move(pSrcDex,pDstDex);
        return true;
    }
    bool MoveUp(int pDex)
    {
        return(move(pDex, pDex+1));
    }
    bool MoveDown(int pDex)
    {
        return(move(pDex, pDex-1));
    }
public:
    QList<Tree_Member_Info>     m_Tree_Members;
};

class AudioEditor : public TabPage
{
    Q_OBJECT  
public:
    explicit AudioEditor(QWidget *parent = 0);
    ~AudioEditor();
    void RefreshAudioListTabel();
    void RefreshSpectrumTabel(int pDex);

private slots:
    void slot_AudioListTabelDoubleClicked(int row, int column);
    void on_AudioAdd_clicked();
    void on_InsertAudio_clicked();
    void on_InsertRecord_clicked(bool checked);

private:
    Ui::AudioEditor *ui;
    MemberInfo       m_MemberInfo;
};

#endif // AUDIOEDITOR_H
