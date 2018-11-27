#ifndef PHYSICALVIDEOPHONE_H
#define PHYSICALVIDEOPHONE_H

#include "physicalbase.h"
#include "basemedia.h"

/*
    <VP>
        <VidSrc Para="bool">true</VidSrc>
        <AudSrc Para="bool">true</AudSrc>
        <VidDst Para="bool">true</VidDst>
        <AudDst Para="bool">true</AudDst>
    </VP>
*/
class PhysicalVP : public PhysicalDevice
{
    Q_OBJECT
public:
    PhysicalVP(QObject *parent = 0);
    ~PhysicalVP();

    quint32 InitPara();
    bool SetValue(QString pValue);

public slots:
    void UpdatePara(QObject* pObject);

    void slot_VPStarted(int pTrack);
    void slot_VPFinished(int pTrack);

public:
    QSignalMapper              *mStartMapper;
    QSignalMapper              *mFinishMapper;

    bool                        mVidSrc, mVidDst;
    MEDIA_PLAYER               *mVidSrcPlayer;
    MEDIA_PLAYER               *mVidDstPlayer;

    bool                        mAudSrc, mAudDst;
    MEDIA_PLAYER               *mAudSrcPlayer;
    MEDIA_PLAYER               *mAudDstPlayer;
};

#endif // LOGICVIDEOPHONE_H
