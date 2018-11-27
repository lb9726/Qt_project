#include "MusicPlayer.h"
#include "DeviceWatcher/DeviceWatcher.h"
#include "AutoConnect.h"
#include "Utility.h"
#include "MultimediaPersistent.h"
#include "UserInterfaceUtility.h"
#include "MusicInformation/MusicInformation.h"
#include <QTimer>
#include <QDBusConnection>
#include <QProcess>
#include <QDir>
#include <QFileInfo>
#include <QDomDocument>
#include <iostream>
using namespace std;

class MusicPlayerPrivate
{
    Q_DISABLE_COPY(MusicPlayerPrivate)
public:
    explicit MusicPlayerPrivate(MusicPlayer *parent);
    ~MusicPlayerPrivate();
    void initialize();
    void connectAllSlots();
    void playMusicIndex(const DeviceWatcherType type, const int index, const int millesimal);
    QList<QString>& getPathList(const DeviceWatcherType type);
    void seekToMillesimal(const int millesimal);
    void setPlayStatus(const MusicPlayerPlayStatus status);
    void setPlayMode(const MusicPlayerPlayMode mode);
    void pauseToggleHandler(const QString& output);
    void timeLengthHandler(const QString &output);
    void millesimalHandler(const QString& output);
    void timePositionHandler(const QString &output);
    void quitHandler();
    void stopHandler();
    void endOfFileHandler();
    void unsupportHandler();
    void exitMusicPlayer();
    void createUSBFileNamesXml();
    void createSDFileNamesXml();
    void musicPlayerSetPlay();
    void musicPlayerSetPause();
    void musicPlayerSetStop();
    void musicPlayerSetExit(const DeviceWatcherType type);
    MusicPlayerPlayMode m_PlayMode;
    MusicPlayerPlayStatus m_PlayStatus;
    QProcess* m_MusicPlayer;
    QList<QString> m_USBPathList;
    QList<QString> m_SDPathList;
    QString m_USBFileNamesXml;
    QString m_SDFileNamesXml;
    DeviceWatcherType m_DiskType;
    QString m_CurrentFilePath;
    MusicInformation m_MusicInformation;
    int m_PlayIndex;
    int m_ElapsedTime;
    int m_EndTime;
    int m_Millesimal;
    QTimer* m_Timer;
private:
    MusicPlayer* m_Parent;
};

void MusicPlayer::musicPlayerRequestFileNames(const DeviceWatcherType type)
{
    switch (type) {
    case DWT_SDDisk: {
        emit onMusicPlayerFileNames(DWT_SDDisk, m_Private->m_SDFileNamesXml);
        break;
    }
    case DWT_USBDisk: {
        emit onMusicPlayerFileNames(DWT_USBDisk, m_Private->m_USBFileNamesXml);
        break;
    }
    default: {
        break;
    }
    }
}

void MusicPlayer::musicPlayerSetPlayModeToggle()
{
    switch (m_Private->m_PlayMode) {
    case MPPM_AllRepeat: {
        musicPlayerSetPlayMode(MPPM_Shuffle);
        break;
    }
    case MPPM_Shuffle: {
        musicPlayerSetPlayMode(MPPM_RepeatOnce);
        break;
    }
    case MPPM_RepeatOnce:
    default: {
        musicPlayerSetPlayMode(MPPM_AllRepeat);
        break;
    }
    }
}

void MusicPlayer::musicPlayerSetPlayMode(const MusicPlayerPlayMode mode)
{
    m_Private->setPlayMode(mode);
}

void MusicPlayer::musicPlayerSetPlayStatusToggle()
{
    switch (m_Private->m_PlayStatus) {
    case MPPS_Play: {
        musicPlayerSetPlayStatus(MPPS_Pause);
        break;
    }
    case MPPS_Unsupport: {
        m_Private->m_Timer->stop();
        m_Private->playMusicIndex(m_Private->m_DiskType, m_Private->m_PlayIndex, 0);
        break;
    }
    case MPPS_Pause:
    default: {
        musicPlayerSetPlayStatus(MPPS_Play);
        break;
    }
    }
}

void MusicPlayer::musicPlayerSetPlayStatus(const MusicPlayerPlayStatus status)
{
    m_Private->m_Timer->stop();
    switch (status) {
    case MPPS_Play: {
        m_Private->musicPlayerSetPlay();
        break;
    }
    case MPPS_Pause: {
        m_Private->musicPlayerSetPause();
        break;
    }
    case MPPS_Stop: {
        m_Private->musicPlayerSetStop();
        break;
    }
    case MPPS_Exit: {
        break;
    }
    default : {
        break;
    }
    }
}

void MusicPlayer::musicPlayerPlayListViewIndex(const DeviceWatcherType type, const int index, const int millesimal)
{
    m_Private->m_Timer->stop();
    m_Private->playMusicIndex(type, index, millesimal);
}

void MusicPlayer::musicPlayerPlayPreviousListViewIndex()
{
    m_Private->m_Timer->stop();
    int lastIndex;
    if (MPPM_Shuffle == m_Private->m_PlayMode) {
        lastIndex = qrand() % m_Private->getPathList(m_Private->m_DiskType).size();
    } else {
        lastIndex = m_Private->m_PlayIndex;
        if ((lastIndex > 0)
                && (lastIndex <= m_Private->getPathList(m_Private->m_DiskType).size() - 1)) {
            --lastIndex;
        } else {
            lastIndex = m_Private->getPathList(m_Private->m_DiskType).size() - 1;
        }
    }
    musicPlayerPlayListViewIndex(m_Private->m_DiskType, lastIndex, 0);
}

void MusicPlayer::musicPlayerPlayNextListViewIndex()
{
    m_Private->m_Timer->stop();
    int lastIndex;
    if (MPPM_Shuffle == m_Private->m_PlayMode) {
        lastIndex = qrand() % m_Private->getPathList(m_Private->m_DiskType).size();
    } else {
        lastIndex = m_Private->m_PlayIndex;
        if ((lastIndex < (m_Private->getPathList(m_Private->m_DiskType).size() - 1))
                && (lastIndex >= 0)) {
            ++lastIndex;
        } else {
            lastIndex = 0;
        }
    }
    musicPlayerPlayListViewIndex(m_Private->m_DiskType, lastIndex, 0);
}

void MusicPlayer::musicPlayerSeekToMillesimal(const int millesimal)
{
    m_Private->m_Timer->stop();
    switch (m_Private->m_MusicPlayer->state()) {
    case QProcess::Running: {
        m_Private->seekToMillesimal(millesimal);
        break;
    }
    default: {
        break;
    }
    }
}

void MusicPlayer::musicPlayerExit()
{
    m_Private->m_Timer->stop();
    m_Private->musicPlayerSetExit(m_Private->m_DiskType);
}

void MusicPlayer::onDeviceWatcherStatus(const DeviceWatcherType type, const DeviceWatcherStatus status)
{
    if (DWT_SDDisk == type) {
        switch (status) {
        case DWS_Empty: {
            m_Private->m_SDPathList.clear();
            break;
        }
        case DWS_Busy: {
            m_Private->m_SDPathList.clear();
            break;
        }
        case DWS_Ready: {
            m_Private->createSDFileNamesXml();
            musicPlayerRequestFileNames(DWT_SDDisk);
            break;
        }
        case DWS_Remove: {
            m_Private->m_SDPathList.clear();
            if (DWT_SDDisk == m_Private->m_DiskType) {
                if (QProcess::NotRunning != m_Private->m_MusicPlayer->state()) {
                    m_Private->m_MusicPlayer->write("pausing_keep_force quit\n\n");
                    m_Private->m_MusicPlayer->waitForBytesWritten();
                    m_Private->m_MusicPlayer->waitForFinished();
                }
                m_Private->m_PlayStatus = MPPS_Exit;
            }
            break;
        }
        default : {
            break;
        }
        }
    } else if (DWT_USBDisk == type) {
        switch (status) {
        case DWS_Empty: {
            m_Private->m_USBPathList.clear();
            break;
        }
        case DWS_Busy: {
            m_Private->m_USBPathList.clear();
            break;
        }
        case DWS_Ready: {
            m_Private->createUSBFileNamesXml();
            musicPlayerRequestFileNames(DWT_USBDisk);
            break;
        }
        case DWS_Remove: {
            m_Private->m_USBPathList.clear();
            if (DWT_USBDisk == m_Private->m_DiskType) {
                if (QProcess::NotRunning != m_Private->m_MusicPlayer->state()) {
                    m_Private->m_MusicPlayer->write("pausing_keep_force quit\n");
                    m_Private->m_MusicPlayer->waitForBytesWritten();
                    m_Private->m_MusicPlayer->waitForFinished();
                }
                m_Private->m_PlayStatus = MPPS_Exit;
            }
            break;
        }
        default : {
            break;
        }
        }
    }
}

void MusicPlayer::onMusicFilePath(const QString &path, const DeviceWatcherType type)
{
    if (DWT_USBDisk == type) {
        m_Private->m_USBPathList.append(path);
    } else if (DWT_SDDisk == type) {
        m_Private->m_SDPathList.append(path);
    }
    //    onMusicPlayerFilePath(type, path);
}

void MusicPlayer::onTimeout()
{
    m_Private->endOfFileHandler();
}

MusicPlayer::MusicPlayer(QObject *parent)
    : QObject(parent)
    , m_Private(new MusicPlayerPrivate(this))
{
}

MusicPlayer::~MusicPlayer()
{
}

MusicPlayerPrivate::MusicPlayerPrivate(MusicPlayer *parent)
    : m_Parent(parent)
{
    initialize();
    connectAllSlots();
}

MusicPlayerPrivate::~MusicPlayerPrivate()
{
    if (QProcess::NotRunning != m_MusicPlayer->state()) {
        m_MusicPlayer->write("pausing_keep_force quit\n");
        m_MusicPlayer->waitForBytesWritten();
        m_MusicPlayer->waitForFinished();
    }
}

void MusicPlayerPrivate::initialize()
{
    m_MusicPlayer = new QProcess(m_Parent);
    m_MusicPlayer->setProcessChannelMode(QProcess::MergedChannels);
    m_Timer = new QTimer(m_Parent);
    m_Timer->setSingleShot(true);
    m_Timer->setInterval(2000);
}

void MusicPlayerPrivate::connectAllSlots()
{
    connectSignalAndSlotByNamesake(g_DeviceWatcher, m_Parent, SLOT(onDeviceWatcherStatus(const int, const int)));
    connectSignalAndSlotByNamesake(g_DeviceWatcher, m_Parent, SLOT(onMusicFilePath(const QString &, const int)));
    Qt::ConnectionType type = static_cast<Qt::ConnectionType>(Qt::UniqueConnection | Qt::AutoConnection);
    QObject::connect(m_MusicPlayer, SIGNAL(started()),
                     m_Parent,      SLOT(onStarted()),
                     type);
    QObject::connect(m_MusicPlayer, SIGNAL(finished(int, QProcess::ExitStatus)),
                     m_Parent,      SLOT(onFinished(int, QProcess::ExitStatus)),
                     type);
    QObject::connect(m_MusicPlayer, SIGNAL(finished(int, QProcess::ExitStatus)),
                     m_Parent,      SLOT(onFinished(int, QProcess::ExitStatus)),
                     type);
    QObject::connect(m_MusicPlayer, SIGNAL(error(QProcess::ProcessError)),
                     m_Parent,      SLOT(onError(QProcess::ProcessError)),
                     type);
    QObject::connect(m_MusicPlayer, SIGNAL(stateChanged(QProcess::ProcessState)),
                     m_Parent,      SLOT(onStateChanged(QProcess::ProcessState)),
                     type);
    QObject::connect(m_MusicPlayer, SIGNAL(readyReadStandardOutput()),
                     m_Parent,      SLOT(onReadyReadStandardOutput()),
                     type);
    QObject::connect(m_MusicPlayer, SIGNAL(readyReadStandardError()),
                     m_Parent,      SLOT(onReadyReadStandardError()),
                     type);
    QObject::connect(m_Timer,  SIGNAL(timeout()),
                     m_Parent, SLOT(onTimeout()),
                     type);
}

void MusicPlayerPrivate::playMusicIndex(const DeviceWatcherType type, const int index, const int millesimal)
{
    QList<QString> temp = getPathList(type);
    if ((temp.size() > index)
            && (QFile(temp.at(index))).exists()) {
        m_CurrentFilePath = temp.at(index);
        m_DiskType = type;
        m_ElapsedTime = 0;
        m_EndTime = 0;
        m_PlayIndex = index;
        m_Millesimal = 0;
        if (0 != millesimal) {
            if (QProcess::NotRunning != m_MusicPlayer->state()) {
                m_MusicPlayer->write("pausing_keep_force quit\n");
                m_MusicPlayer->waitForBytesWritten();
                m_MusicPlayer->waitForFinished();
            }
        }
        if (QProcess::NotRunning == m_MusicPlayer->state()) {
            QString cmd = QString("mplayer");
            QStringList arguments;
            arguments << QString("-slave");
            arguments << QString("-idle");
            arguments << QString("-quiet");
            arguments << QString("-ao");
            arguments << QString("alsa");
            arguments << QString("-vo");
            arguments << QString("null");
            if (0 != millesimal) {
                arguments << QString("-ss");
                arguments << QString::number(millesimal);
            }
            m_MusicPlayer->start(cmd, arguments);
            m_MusicPlayer->waitForStarted();
        }
        QString cmd = QString("loadfile \"") + m_CurrentFilePath + QString("\"\n");
        m_MusicPlayer->write(cmd.toLocal8Bit().constData());
        m_MusicPlayer->waitForBytesWritten();
        emit m_Parent->onMusicPlayerPlayMode(m_PlayMode);
        m_PlayStatus = MPPS_Start;
        emit m_Parent->onMusicPlayerPlayStatus(m_DiskType, m_PlayStatus);
        QFileInfo fileInfo(m_CurrentFilePath);
        m_MusicInformation.parserFilePath(m_CurrentFilePath);
        emit m_Parent->onMusicPlayerElapsedInformation(m_ElapsedTime, m_Millesimal);
        emit m_Parent->onMusicPlayerID3TagChange(m_DiskType,
                                                 m_PlayIndex,
                                                 fileInfo.filePath(),
                                                 m_MusicInformation.getTitle(),
                                                 m_MusicInformation.getArtist(),
                                                 m_MusicInformation.getAlbum(),
                                                 m_EndTime);
        MultimediaPersistent::setMusicPathInfo(m_DiskType, fileInfo.filePath() + QString("/") + fileInfo.created().toString(QString("yyyyMMddhhmmss")) + fileInfo.lastModified().toString(QString("yyyyMMddhhmmss")) + QString::number(fileInfo.size()));
        MultimediaPersistent::setMultimediaType(m_DiskType, MT_Music);
        MultimediaPersistent::setMusicMillesmial(m_DiskType, 0);
    }
}

QList<QString> &MusicPlayerPrivate::getPathList(const int type)
{
    switch (type) {
    case DWT_SDDisk: {
        return m_SDPathList;
        break;
    }
    case DWT_USBDisk:
    default: {
        return m_USBPathList;
        break;
    }
    }
}

void MusicPlayerPrivate::seekToMillesimal(const int millesimal)
{
    if ((MPPS_Play == m_PlayStatus)
            || (MPPS_Pause == m_PlayStatus)) {
        QString cmd("seek %1.%2 1\n");
        cmd = cmd.arg(millesimal / 10, 3, 10, QChar(' ')).arg(millesimal % 10, 1, 10, QChar('0'));
        m_MusicPlayer->write(cmd.toLocal8Bit().constData());
        m_MusicPlayer->waitForBytesWritten();
        setPlayStatus(MPPS_Play);
    } else if (MPPS_Stop == m_PlayStatus) {
        m_Timer->stop();
        playMusicIndex(m_DiskType, m_PlayIndex, 0);
    }
}

void MusicPlayerPrivate::setPlayStatus(const MusicPlayerPlayStatus status)
{
    if (status != m_PlayStatus) {
        m_PlayStatus = status;
    }
    emit m_Parent->onMusicPlayerPlayStatus(m_DiskType, m_PlayStatus);
}

void MusicPlayerPrivate::setPlayMode(const MusicPlayerPlayMode mode)
{
    if (mode != m_PlayMode) {
        m_PlayMode = mode;
    }
    emit m_Parent->onMusicPlayerPlayMode(m_PlayMode);
}

void MusicPlayerPrivate::pauseToggleHandler(const QString &output)
{
    if (output.contains(QString("yes"))) {
        setPlayStatus(MPPS_Pause);
    } else if (output.contains(QString("no"))) {
        setPlayStatus(MPPS_Play);
    }
}

void MusicPlayerPrivate::timeLengthHandler(const QString &output)
{
    int start = output.indexOf(QChar('=')) + 1;
    int end = output.indexOf(QChar('\n'));
    if ((-1 != start)
            && (-1 != end)) {
        m_EndTime = static_cast<int>(output.mid(start, end - start).toFloat());
    } else {
        m_EndTime = 0;
    }
    QFileInfo fileInfo(m_CurrentFilePath);
    m_MusicInformation.parserFilePath(m_CurrentFilePath);
    emit m_Parent->onMusicPlayerID3TagChange(m_DiskType,
                                             m_PlayIndex,
                                             fileInfo.filePath(),
                                             m_MusicInformation.getTitle(),
                                             m_MusicInformation.getArtist(),
                                             m_MusicInformation.getAlbum(),
                                             m_EndTime);
    m_PlayStatus = MPPS_Play;
    emit m_Parent->onMusicPlayerPlayStatus(m_DiskType, m_PlayStatus);
}

void MusicPlayerPrivate::millesimalHandler(const QString &output)
{
    int start = output.indexOf(QChar('=')) + 1;
    int end = output.indexOf(QChar('\n'));
    if ((-1 != start)
            && (-1 != end)) {
        bool ok(false);
        int millesimal = static_cast<int>(output.mid(start, end - start).toFloat(&ok) * 1000);
        if (ok) {
            m_Millesimal = millesimal;
        }
    }
}

void MusicPlayerPrivate::timePositionHandler(const QString &output)
{
    if (output.startsWith(QString("ANS_TIME_POSITION="), Qt::CaseSensitive)) {
        int start = output.indexOf(QChar('=')) + 1;
        int end = output.indexOf(QChar('\n'));
        if ((-1 != start)
                && (-1 != end)) {
            bool ok(false);
            int elapsed = output.mid(start, end - start).toInt(&ok);
            if (ok) {
                if (((qAbs(m_ElapsedTime - elapsed) > 5))
                        || (elapsed % 5)) {
                    MultimediaPersistent::setMusicMillesmial(m_DiskType, elapsed);
                }
                m_ElapsedTime = elapsed;
            }
        }
        emit m_Parent->onMusicPlayerElapsedInformation(m_ElapsedTime, m_Millesimal);
    }
}

void MusicPlayerPrivate::quitHandler()
{
    setPlayStatus(MPPS_Exit);
}

void MusicPlayerPrivate::stopHandler()
{
    m_ElapsedTime = 0;
    m_Millesimal = 0;
    setPlayStatus(MPPS_Stop);
    emit m_Parent->onMusicPlayerElapsedInformation(m_ElapsedTime, m_Millesimal);
}

void MusicPlayerPrivate::endOfFileHandler()
{
    switch (m_PlayStatus) {
    case MPPS_Start:
    case MPPS_Unsupport:
    case MPPS_Pause:
    case MPPS_Stop:
    case MPPS_Play: {
        switch (m_PlayMode) {
        case MPPM_AllRepeat: {
            if ((getPathList(m_DiskType).size() - 1) > m_PlayIndex) {
                ++m_PlayIndex;
            } else {
                m_PlayIndex = 0;
            }
            break;
        }
        case MPPM_RepeatOnce: {
            break;
        }
        case MPPM_Shuffle:
        default : {
            m_PlayIndex = qrand() % getPathList(m_DiskType).size();
            break;
        }
        }
        playMusicIndex(m_DiskType, m_PlayIndex, 0);
        break;
    }
    default: {
        break;
    }
    }
}

void MusicPlayerPrivate::unsupportHandler()
{
    m_EndTime = 0;
    QFileInfo fileInfo(m_CurrentFilePath);
    emit m_Parent->onMusicPlayerID3TagChange(m_DiskType,
                                             m_PlayIndex,
                                             fileInfo.filePath(),
                                             QString(),
                                             QString(),
                                             QString(),
                                             m_EndTime);
    m_ElapsedTime = 0;
    m_Millesimal = 0;
    emit m_Parent->onMusicPlayerElapsedInformation(m_ElapsedTime, m_Millesimal);
    setPlayStatus(MPPS_Unsupport);
    m_Timer->start();
}

void MusicPlayerPrivate::exitMusicPlayer()
{
    if (QProcess::NotRunning != m_MusicPlayer->state()) {
        m_MusicPlayer->write("pausing_keep_force quit\n");
        m_MusicPlayer->waitForBytesWritten();
        m_MusicPlayer->waitForFinished();
        setPlayStatus(MPPS_Exit);
    }
}

void MusicPlayerPrivate::createUSBFileNamesXml()
{
    QDomDocument domDocument;
    domDocument.clear();
    domDocument.appendChild(domDocument.createElement(QString("MusicPlayer")));
    QDomElement root = domDocument.firstChildElement(QString("MusicPlayer"));
    QDomElement fileNames;
    QDomElement info;
    fileNames = domDocument.createElement(QString("USBFileNames"));
    root.appendChild(fileNames);
    m_USBFileNamesXml.clear();
    QString usbPersistantPathInfo = MultimediaPersistent::getMusicPathInfo(DWT_USBDisk);
    QStringList stringList = usbPersistantPathInfo.split(QChar('/'));
    QString usbPersistantPath = usbPersistantPathInfo.left(usbPersistantPathInfo.length() - stringList.last().length() - 1);
    QString persistantIndex("");
    QFileInfo fileInfo;
    int i;
    for (i = 0; i < m_USBPathList.size(); ++i) {
        info = domDocument.createElement(QString("Index:" + QString::number(i)));
        fileNames.appendChild(info);
        fileInfo.setFile(m_USBPathList.at(i));
        info.appendChild(domDocument.createTextNode(fileInfo.filePath()));
        if (persistantIndex.isEmpty()) {
            if (fileInfo.filePath() == usbPersistantPath) {
                if (usbPersistantPathInfo == (fileInfo.filePath() + QString("/") + fileInfo.created().toString(QString("yyyyMMddhhmmss")) + fileInfo.lastModified().toString(QString("yyyyMMddhhmmss")) + QString::number(fileInfo.size()))) {
                    persistantIndex = QString::number(i);
                }
            }
        }
    }
    QDomElement persistant = domDocument.createElement(QString("USBPersistant"));
    root.appendChild(persistant);
    unsigned char multimediaType = MultimediaPersistent::getMultimediaType(DWT_USBDisk);
    int musicMillesmial = 0;
    if (persistantIndex.isEmpty()) {
        if (0 == i) {
            persistantIndex = QString("-1");
            multimediaType = MT_Idle;
            MultimediaPersistent::setMultimediaType(DWT_USBDisk, MT_Image);
        } else {
            if (0 == multimediaType) {
                multimediaType = MT_Music;
                MultimediaPersistent::setMultimediaType(DWT_USBDisk, multimediaType);
            }
            persistantIndex = QString("0");
        }
    } else {
        musicMillesmial = MultimediaPersistent::getMusicMillesmial(DWT_USBDisk);
    }
    QString data = persistantIndex + QString("-") + QString::number(musicMillesmial);
    persistant.appendChild(domDocument.createTextNode(data));
    QDomElement type = domDocument.createElement(QString("USBType"));
    root.appendChild(type);
    type.appendChild(domDocument.createTextNode(QString::number(multimediaType)));
    m_USBFileNamesXml = domDocument.toString();
}

void MusicPlayerPrivate::createSDFileNamesXml()
{
    QDomDocument domDocument;
    domDocument.clear();
    domDocument.appendChild(domDocument.createElement(QString("MusicPlayer")));
    QDomElement root = domDocument.firstChildElement(QString("MusicPlayer"));
    QDomElement fileNames;
    QDomElement info;
    fileNames = domDocument.createElement(QString("SDFileNames"));
    root.appendChild(fileNames);
    m_SDFileNamesXml.clear();
    QString sdPersistantPathInfo = MultimediaPersistent::getMusicPathInfo(DWT_SDDisk);
    QStringList stringList = sdPersistantPathInfo.split(QChar('/'));
    QString sdPersistantPath = sdPersistantPathInfo.left(sdPersistantPathInfo.length() - stringList.last().length() - 1);
    QString persistantIndex("");
    QFileInfo fileInfo;
    int i;
    for (i = 0; i < m_SDPathList.size(); ++i) {
        info = domDocument.createElement(QString("Index:" + QString::number(i)));
        fileNames.appendChild(info);
        fileInfo.setFile(m_SDPathList.at(i));
        info.appendChild(domDocument.createTextNode(fileInfo.filePath()));
        if (persistantIndex.isEmpty()) {
            if (fileInfo.filePath() == sdPersistantPath) {
                if (sdPersistantPathInfo == (fileInfo.filePath() + QString("/") + fileInfo.created().toString(QString("yyyyMMddhhmmss")) + fileInfo.lastModified().toString(QString("yyyyMMddhhmmss")) + QString::number(fileInfo.size()))) {
                    persistantIndex = QString::number(i);
                }
            }
        }
    }
    QDomElement persistant = domDocument.createElement(QString("SDPersistant"));
    root.appendChild(persistant);
    unsigned char multimediaType = MultimediaPersistent::getMultimediaType(DWT_SDDisk);
    int musicMillesmial = 0;
    if (persistantIndex.isEmpty()) {
        if (0 == i) {
            persistantIndex = QString("-1");
            multimediaType = MT_Idle;
            MultimediaPersistent::setMultimediaType(DWT_SDDisk, MT_Image);
        } else {
            if (0 == multimediaType) {
                multimediaType = MT_Music;
                MultimediaPersistent::setMultimediaType(DWT_SDDisk, multimediaType);
            }
            persistantIndex = QString("0");
        }
    } else {
        musicMillesmial = MultimediaPersistent::getMusicMillesmial(DWT_SDDisk);
    }
    QString data = persistantIndex + QString("-") + QString::number(musicMillesmial);
    persistant.appendChild(domDocument.createTextNode(data));
    QDomElement type = domDocument.createElement(QString("SDType"));
    root.appendChild(type);
    type.appendChild(domDocument.createTextNode(QString::number(multimediaType)));
    m_SDFileNamesXml = domDocument.toString();
}

void MusicPlayerPrivate::musicPlayerSetPlay()
{
    switch (m_PlayStatus) {
    case MPPS_Pause: {
        m_MusicPlayer->write("u\n");
        m_MusicPlayer->waitForBytesWritten();
        break;
    }
    default: {
        break;
    }
    }
}

void MusicPlayerPrivate::musicPlayerSetPause()
{
    switch (m_PlayStatus) {
    case MPPS_Start:
    case MPPS_Play: {
        m_MusicPlayer->write("pausing_keep_force pause\n");
        m_MusicPlayer->waitForBytesWritten();
    }
    case MPPS_Pause: {
        break;
    }
    default: {
        return ;
        break;
    }
    }
}

void MusicPlayerPrivate::musicPlayerSetStop()
{
    if ((MPPS_Play == m_PlayStatus)
            || (MPPS_Pause == m_PlayStatus)) {
        m_MusicPlayer->write("stop\n");
        m_MusicPlayer->waitForBytesWritten();
    } else {
        stopHandler();
    }
}

void MusicPlayerPrivate::musicPlayerSetExit(const DeviceWatcherType type)
{
    if (type == m_DiskType) {
        exitMusicPlayer();
    }
}

void MusicPlayer::onReadyReadStandardOutput()
{
    while (m_Private->m_MusicPlayer->canReadLine()) {
        QString output(m_Private->m_MusicPlayer->readLine());
        if (output.contains(QString("ANS_LENGTH="))) {
            m_Private->timeLengthHandler(output);
        } else if (output.contains(QString("ANS_PERCENT_POSITION="))) {
            m_Private->millesimalHandler(output);
        } else if (output.contains(QString("ANS_TIME_POSITION="))) {
            m_Private->timePositionHandler(output);
        } else if (output.contains(QString("ANS_pause"))) {
            m_Private->pauseToggleHandler(output);
        } else if (output.contains(QString("EOF code: 1"))) {
            m_Private->setPlayStatus(MPPS_Stop);
            m_Private->m_Timer->start();
        } else if (output.contains(QString("EOF code: 4"))) {
            m_Private->stopHandler();
        } else if (output.contains(QString("incorrect streams"))) {
            m_Private->unsupportHandler();
        }/* else if (output.contains(QString("seek-begin"))) {
        } */else if ((output.contains(QString("seek-finish"))
                      || (output.contains(QString("seek-error"))))) {
            emit onMusicPlayerPlayStatus(m_Private->m_DiskType, MPPS_SeekFinish);
        } else {
            if (output != QString("\n")) {
                qDebug() << "output" << output;
            }
        }
    }
}

void MusicPlayer::onReadyReadStandardError()
{
    qWarning() << __PRETTY_FUNCTION__;
    while (m_Private->m_MusicPlayer->canReadLine()) {
        QString output(m_Private->m_MusicPlayer->readLine());
        qWarning() << __PRETTY_FUNCTION__ << output;
    }
}

void MusicPlayer::onStarted()
{
    qWarning() << "MusicPlayer::onStarted" << m_Private->m_MusicPlayer->state();
}

void MusicPlayer::onFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qWarning() << "MusicPlayer::onFinished" << exitCode << exitStatus;
}

void MusicPlayer::onError(QProcess::ProcessError error)
{
    qWarning() << "MusicPlayer::onError" << error;
}

void MusicPlayer::onStateChanged(QProcess::ProcessState state)
{
    qWarning() << "MusicPlayer::onStateChanged" << state;
}

