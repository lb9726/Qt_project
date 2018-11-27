#include "physicalvp.h"
#ifdef IDE
    #include "tabpage.h"
    #include "mdppage.h"
#endif
#include "deviceinc.h"
#include "logicthememanager.h"
#include "physicalnet.h"

PhysicalVP::PhysicalVP(QObject *parent)
    :PhysicalDevice(parent)
{
    mVidSrcPlayer = 0;
    mVidDstPlayer = 0;
    mAudSrcPlayer = 0;
    mAudDstPlayer = 0;

    mStartMapper = new QSignalMapper;
    connect(mStartMapper, SIGNAL(mapped(int)), this, SLOT(slot_VPStarted(int)));

    mFinishMapper = new QSignalMapper;
    connect(mFinishMapper, SIGNAL(mapped(int)), this, SLOT(slot_VPFinished(int)));
}

PhysicalVP::~PhysicalVP()
{
}

quint32 PhysicalVP::InitPara()
{
    QDomNodeList tmpParaNodeList = m_ParaElement.childNodes();
    if(tmpParaNodeList.count() < 1)
        return 0;
    quint32 validcount = 0;
    QHBoxLayout* tmpHBoxLayout;
    for(int i=0;i<tmpParaNodeList.count();i++)
    {
        QDomElement tmpElement = tmpParaNodeList.at(i).toElement();
        if(tmpElement.isNull())
            continue;
        QString tmpName = tmpElement.tagName();
        if(!tmpName.compare("VidSrc"))
        {
            QString tmpValue = tmpElement.text();
            mVidSrc = (!tmpValue.compare("false"))?(false):(true);
            tmpHBoxLayout = CreateBool(tmpName, tmpValue);
            if(mVidSrc)
            {
                mVidSrcPlayer = new MEDIA_PLAYER;
                connect(mVidSrcPlayer, SIGNAL(sStarted()), mStartMapper, SLOT(map()));
                mStartMapper->setMapping(mVidSrcPlayer, AO_VPSRC);
                connect(mVidSrcPlayer, SIGNAL(sFinished()), mFinishMapper, SLOT(map()));
                mFinishMapper->setMapping(mVidSrcPlayer, AO_VPSRC);
                mVidSrcPlayer->Add(AO_VPSRC, QString("VPSRC"), 0, false, 100, true);
            }
        }
        else if(!tmpName.compare("VidDst"))
        {
            QString tmpValue = tmpElement.text();
            mVidDst = (!tmpValue.compare("false"))?(false):(true);
            tmpHBoxLayout = CreateBool(tmpName, tmpValue);
            if(mVidDst)
            {
                mVidDstPlayer = new MEDIA_PLAYER;
                connect(mVidDstPlayer, SIGNAL(sStarted()), mStartMapper, SLOT(map()));
                mStartMapper->setMapping(mVidDstPlayer, AO_VPDST);
                connect(mVidDstPlayer, SIGNAL(sFinished()), mFinishMapper, SLOT(map()));
                mFinishMapper->setMapping(mVidDstPlayer, AO_VPDST);
                mVidDstPlayer->Add(AO_VPSRC, QString("VPDST"), 0, false, 100, true);
            }
        }
        else if(!tmpName.compare("AudSrc"))
        {
            QString tmpValue = tmpElement.text();
            mAudSrc = (!tmpValue.compare("false"))?(false):(true);
            tmpHBoxLayout = CreateBool(tmpName, tmpValue);
            if(mAudSrc)
            {
                mAudSrcPlayer = new MEDIA_PLAYER;
                connect(mAudSrcPlayer, SIGNAL(sStarted()), mStartMapper, SLOT(map()));
                mStartMapper->setMapping(mAudSrcPlayer, AO_APSRC);
                connect(mAudSrcPlayer, SIGNAL(sFinished()), mFinishMapper, SLOT(map()));
                mFinishMapper->setMapping(mAudSrcPlayer, AO_APSRC);
                mAudSrcPlayer->Add(AO_VPSRC, QString("APSRC"), 0, false, 100, true);
            }
        }
        else if(!tmpName.compare("AudDst"))
        {
            QString tmpValue = tmpElement.text();
            mAudDst = (!tmpValue.compare("false"))?(false):(true);
            tmpHBoxLayout = CreateBool(tmpName, tmpValue);
            if(mAudDst)
            {
                mAudDstPlayer = new MEDIA_PLAYER;
                connect(mAudDstPlayer, SIGNAL(sStarted()), mStartMapper, SLOT(map()));
                mStartMapper->setMapping(mAudDstPlayer, AO_APDST);
                connect(mAudDstPlayer, SIGNAL(sFinished()), mFinishMapper, SLOT(map()));
                mFinishMapper->setMapping(mAudDstPlayer, AO_APDST);
                mAudDstPlayer->Add(AO_VPSRC, QString("APDST"), 0, false, 100, true);
            }
        }
        else
            continue;
        if(!tmpHBoxLayout)
            continue;
        validcount++;
        m_SettingGroup->m_VLayout->addWidget(CreateLine());
        m_SettingGroup->m_VLayout->addLayout(tmpHBoxLayout);
    }
    validcount++;
    return validcount;
}

void PhysicalVP::UpdatePara(QObject* pObject)
{
    QString tmpObjectName = pObject->objectName();
    QDomElement tmpElement = m_ParaElement.firstChildElement(tmpObjectName);
    if(tmpElement.isNull())
        return;
    QString tmpName = tmpElement.tagName();
    if(!tmpName.compare("VidSrc"))
    {
        Switch *tmpComponent = VAR_CAST<Switch *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyBool(tmpComponent, tmpElement, mVidSrc, tmpComponent->isChecked()))
            {
            }
            else
            {
                if(mVidSrc)
                {
                    if(!mVidSrcPlayer)
                    {
                        mVidSrcPlayer = new MEDIA_PLAYER;
                        connect(mVidSrcPlayer, SIGNAL(sStarted()), mStartMapper, SLOT(map()));
                        mStartMapper->setMapping(mVidSrcPlayer, AO_VPSRC);
                        connect(mVidSrcPlayer, SIGNAL(sFinished()), mFinishMapper, SLOT(map()));
                        mFinishMapper->setMapping(mVidSrcPlayer, AO_VPSRC);
                        mVidSrcPlayer->Add(AO_VPSRC, QString("VPSRC"), 0, false, 100, true);
                    }
                }
                else
                {
                    if(mVidSrcPlayer)
                    {
                        mVidSrcPlayer->deleteLater();
                        mVidSrcPlayer = 0;
                    }
                }
            }
        }
    }
    else if(!tmpName.compare("VidDst"))
    {
        Switch *tmpComponent = VAR_CAST<Switch *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyBool(tmpComponent, tmpElement, mVidDst, tmpComponent->isChecked()))
            {
            }
            else
            {
                if(mVidDst)
                {
                    if(!mVidDstPlayer)
                    {
                        mVidDstPlayer = new MEDIA_PLAYER;
                        connect(mVidDstPlayer, SIGNAL(sStarted()), mStartMapper, SLOT(map()));
                        mStartMapper->setMapping(mVidDstPlayer, AO_VPDST);
                        connect(mVidDstPlayer, SIGNAL(sFinished()), mFinishMapper, SLOT(map()));
                        mFinishMapper->setMapping(mVidDstPlayer, AO_VPDST);
                        mVidDstPlayer->Add(AO_VPSRC, QString("VPDST"), 0, false, 100, true);
                    }
                }
                else
                {
                    if(mVidDstPlayer)
                    {
                        mVidDstPlayer->deleteLater();
                        mVidDstPlayer = 0;
                    }
                }
            }
        }
    }
    else if(!tmpName.compare("AudSrc"))
    {
        Switch *tmpComponent = VAR_CAST<Switch *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyBool(tmpComponent, tmpElement, mAudSrc, tmpComponent->isChecked()))
            {
            }
            else
            {
                if(mAudSrc)
                {
                    if(!mAudSrcPlayer)
                    {
                        mAudSrcPlayer = new MEDIA_PLAYER;
                        connect(mAudSrcPlayer, SIGNAL(sStarted()), mStartMapper, SLOT(map()));
                        mStartMapper->setMapping(mAudSrcPlayer, AO_APSRC);
                        connect(mAudSrcPlayer, SIGNAL(sFinished()), mFinishMapper, SLOT(map()));
                        mFinishMapper->setMapping(mAudSrcPlayer, AO_APSRC);
                        mAudSrcPlayer->Add(AO_VPSRC, QString("APSRC"), 0, false, 100, true);
                    }
                }
                else
                {
                    if(mAudSrcPlayer)
                    {
                        mAudSrcPlayer->deleteLater();
                        mAudSrcPlayer = 0;
                    }
                }
            }
        }
    }
    else if(!tmpName.compare("AudDst"))
    {
        Switch *tmpComponent = VAR_CAST<Switch *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyBool(tmpComponent, tmpElement, mAudDst, tmpComponent->isChecked()))
            {
            }
            else
            {
                if(mAudDst)
                {
                    if(!mAudDstPlayer)
                    {
                        mAudDstPlayer = new MEDIA_PLAYER;
                        connect(mAudDstPlayer, SIGNAL(sStarted()), mStartMapper, SLOT(map()));
                        mStartMapper->setMapping(mAudDstPlayer, AO_APDST);
                        connect(mAudDstPlayer, SIGNAL(sFinished()), mFinishMapper, SLOT(map()));
                        mFinishMapper->setMapping(mAudDstPlayer, AO_APDST);
                        mAudDstPlayer->Add(AO_VPSRC, QString("APDST"), 0, false, 100, true);
                    }
                }
                else
                {
                    if(mAudDstPlayer)
                    {
                        mAudDstPlayer->deleteLater();
                        mAudDstPlayer = 0;
                    }
                }
            }
        }
    }
}

void PhysicalVP::slot_VPStarted(int pTrack)
{
    IDE_DEBUG(QString("VP Track[%1] is started!").arg(pTrack));
    switch(pTrack)
    {
        case AO_VPSRC:
        {
            break;
        }
        case AO_VPDST:
        {
            break;
        }
        case AO_APSRC:
        {
            break;
        }
        case AO_APDST:
        {
            break;
        }
        default:
            break;
    }
}

void PhysicalVP::slot_VPFinished(int pTrack)
{
    IDE_DEBUG(QString("VP Track[%1] is finished!").arg(pTrack));
    switch(pTrack)
    {
        case AO_VPSRC:
        {
            break;
        }
        case AO_VPDST:
        {
            break;
        }
        case AO_APSRC:
        {
            break;
        }
        case AO_APDST:
        {
            break;
        }
        default:
            break;
    }
}

bool PhysicalVP::SetValue(QString pValue)
{
    if(pValue.isEmpty())
        return true;
    if(!pValue.compare("start", Qt::CaseInsensitive))  //>@指根据配置文件开启视频对讲发送端与接收端
    {
        if(mVidSrcPlayer)
            mVidSrcPlayer->SetEnable(true);
        if(mVidDstPlayer)
            mVidDstPlayer->SetEnable(true);
        if(mAudSrcPlayer)
            mAudSrcPlayer->SetEnable(true);
        if(mAudDstPlayer)
            mAudDstPlayer->SetEnable(true);
    }
    else if(!pValue.compare("stop", Qt::CaseInsensitive))
    {
        if(mVidSrcPlayer)
            mVidSrcPlayer->SetEnable(false);
        if(mVidDstPlayer)
            mVidDstPlayer->SetEnable(false);
        if(mAudSrcPlayer)
            mAudSrcPlayer->SetEnable(false);
        if(mAudDstPlayer)
            mAudDstPlayer->SetEnable(false);
    }
    else
    return true;
}


