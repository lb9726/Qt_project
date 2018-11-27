#include "mediascreen.h"
#include <QLocale>
#include <QHostInfo>
#include <QList>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QDateTime>
#include "define.h"
#include <QQmlComponent>
#include <QQuickItem>

MediaScreen::MediaScreen(QObject *parent) : QObject(parent)
  , index(-1)
  , preIndex(-1)
  , isInsertUdiskFlag(false)
{
    getLocalIp();
#if defined(Q_OS_WIN32)
    nativeFilter = 0;
#endif
}

//void MediaScreen::changeUi()
//{
//    ConfigureHolder* tmpconfigure = configureSerialer;
//    QQmlApplicationEngine* tmpEngine = engine;

//    engine = new QQmlApplicationEngine();
//    configureSerialer = new ConfigureHolder(engine);
//    content = engine->rootContext();

//    tmpconfigure->deleteLater();
//    tmpEngine->deleteLater();

//#if defined(Q_OS_WIN32)
//    content->setContextProperty("UsbHelper", nativeFilter->usbHelper); // 设置Qml的上下文属性，可以直接在qml中引用
//#elif defined(Q_OS_LINUX)
//    content->setContextProperty("UsbHelper", &mUsbHelper);
//#endif
//    content->setContextProperty("ConfigureSerialer", configureSerialer);
//    content->setContextProperty("Ctranslator", CTranslator::instance());
//    content->setContextProperty("MediaScreen", this);

//    if(0 == index || 1 == index || 2 == index)
//    {
//        CTranslator::instance()->load(index);
//        emit clearAudioParament(index);
//        engine->load(QUrl(QLatin1String("qrc:/main.qml")));
//    }
//    else if (3 == index)
//    {
//        emit clearAudioParament(index);
//        engine->load(QUrl(QLatin1String("qrc:///russian/RussianMain.qml")));
//    }
//}

void MediaScreen::changeUi()
{
    ConfigureHolder* tmpconfigure = configureSerialer;
    QQmlApplicationEngine* tmpEngine = engine;

    engine = new QQmlApplicationEngine();
    if (engine)
    {
        configureSerialer = new ConfigureHolder(engine);
        content = engine->rootContext();
    }

    tmpconfigure->deleteLater();
    tmpEngine->deleteLater();

#if defined(Q_OS_WIN32)
    if (content)
    {
       content->setContextProperty("UsbHelper", nativeFilter->usbHelper); // 设置Qml的上下文属性，可以直接在qml中引用
    }

#elif defined(Q_OS_LINUX)
    if (content)
    {
        content->setContextProperty("UsbHelper", &mUsbHelper);
    }
#endif
    if (content)
    {
        content->setContextProperty("ConfigureSerialer", configureSerialer);
        content->setContextProperty("Ctranslator", CTranslator::instance());
        content->setContextProperty("MediaScreen", this);
    }
    if(0 == index || 1 == index || 2 == index)
    {
        CTranslator::instance()->load(index);
        emit clearAudioParament(index);
        if (engine)
        {
            engine->load(QUrl(QLatin1String("qrc:/main.qml")));
        }
    }
    else if (3 == index)
    {
        emit clearAudioParament(index);
        if (engine)
        {
            engine->load(QUrl(QLatin1String("qrc:///russian/RussianMain.qml")));
        }
    }
    QObject* tmpobj =  engine->rootObjects().value(0);
    if (tmpobj)
    {
        qDebug()<<__PRETTY_FUNCTION__<<"tmpobj = "<<tmpobj;
        QQuickItem* tmpQuick = tmpobj->findChild<QQuickItem *>("parameterSetting");
        if (tmpQuick)
        {
            qDebug()<<__PRETTY_FUNCTION__<<"tmpQuick = "<<tmpQuick;
            qDebug()<<"tmpQuick.width = "<<tmpQuick->width();
            qDebug()<<"tmpQuick.height = "<<tmpQuick->height();
        }
    }
}

void MediaScreen::setIndex(int i)
{
    qDebug()<<__FUNCTION__<<"()parament i = "<< i;
    index = i;
    qDebug()<<"SetIndex index = "<< index;
}

QVariant MediaScreen::getIndex()
{
    qDebug()<<__FUNCTION__<<"()index = "<<index;
    return index;
}

void MediaScreen::getScrollTextLength()
{
    emit getScrollTextLengthSignal();
}

int MediaScreen::getSystemLanguage()
{
    qDebug()<<__FUNCTION__<<"()local language";
    QLocale local;
    if (QLocale::English == local.language())
    {
        qDebug()<<"is English";
        return 2;
    }
    else if (QLocale::Chinese == local.language())
    {
        qDebug()<<"is Chinese";
        return 0;
    }
    else
    {
        return 2;
    }
}

#if defined(Q_OS_WIN32)
UsbEventFilter *MediaScreen::getPoint()
{
    if (0 != nativeFilter)
    {
        return nativeFilter;
    }
    return 0;
}

#elif defined(Q_OS_LINUX)
void MediaScreen::usbDetected()
{
    mUsbHelper.detectUsbIsInsert("/media/");
}

#endif

void MediaScreen::setPreIndex(int i)
{
    qDebug()<<__FUNCTION__<<"() before PreIndex is "<< preIndex;
    preIndex = index; // 记录上一次的preIndex的值
    qDebug()<<__FUNCTION__<<"() after PreIndex is "<< preIndex;
    setIndex(i);
}

QVariant MediaScreen::getPreIndex()
{
    qDebug()<<__FUNCTION__<<"() PreIndex is "<< preIndex;
    return preIndex;
}

QString MediaScreen::getIndexIpString(int index)
{
    if (ipString.isEmpty())
    {
        return QString();
    }
    else
    {
        QStringList tmpList = ipString.split(".");
        if (index < 1)
        {
            return QString();
        }
        else if (index <= tmpList.length())
        {
            qDebug()<<"tmpList.at(index -1 ) = " << tmpList.at(index - 1);
            return tmpList.at(index - 1);
        }
        else
        {
            return QString();
        }
    }
}

MediaScreen::~MediaScreen()
{
    #if defined(Q_OS_WIN32)
    if (nativeFilter)
    {
        delete nativeFilter;
        nativeFilter = 0;
    }
    #elif defined(Q_OS_LINUX)

    #endif
    if (configureSerialer)
    {
        configureSerialer->deleteLater();
        configureSerialer = 0;
    }
}

void MediaScreen::getLocalIp()
{
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i)
    {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                ipAddressesList.at(i).toIPv4Address())
        {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    ipString = ipAddress;
    qDebug()<<"ipString = "<<ipString;
}

QVariant MediaScreen::getModel(int index)
{
    QStringList tmpVar;
    switch (index) {
    case 0:
        tmpVar<<"China Mobile"<<"China Unicom";
        break;
    case 1:
        tmpVar<<"M1"<<"SingTel"<<"Starhub";
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        break;
    default:
        break;
    }
    qDebug()<<"tmpVar = "<<tmpVar;
    return tmpVar;
}

QString MediaScreen::getSplitTime(int index)
{
    QString timeStr = getTimeFormat(false);
    QStringList list = timeStr.split(":");
    if (index > list.length())
    {
        return QString();
    }
    else if (index < 1)
    {
        return QString();
    }
    else
    {
        qDebug()<<list.at(index - 1);
        return list.at(index - 1);
    }
}

QString MediaScreen::getSplitDate(int index)
{
    QString dateStr = getDateFormat(true);
    QStringList list = dateStr.split("-");
    if (index > list.length())
    {
        return QString();
    }
    else if (index < 1)
    {
        return QString();
    }
    else
    {
        qDebug()<<list.at(index - 1);
        return list.at(index - 1);
    }
}

QString MediaScreen::getDateFormat(bool isYmd)
{
    QString date = "";
    date.clear();
    if (isYmd)
    {
        date = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    }
    else
    {
        date = QDateTime::currentDateTime().toString("MM-dd-yyyy");
    }
    //    qDebug()<< "date = "<< date;
    return date;
}

QString MediaScreen::getTimeFormat(bool is12)
{
    QString timelocal = "";
    timelocal.clear();
    if (is12)
    {
        timelocal = QDateTime::currentDateTime().toString("hh:mm:ss a");
        timelocal =  timelocal.split(" ").at(0);
    }
    else
    {
        timelocal = QDateTime::currentDateTime().toString("HH:mm:ss");
    }
    //    qDebug()<< "timelocal = "<< timelocal;
    return timelocal;
}

void MediaScreen::setUdiskIsInsertFlag(bool flag, QString udiskIndex)
{
    isInsertUdiskFlag = flag;
    udiskIndexStr = udiskIndex;
    qDebug()<<"isInsertUdiskFlag = "<< isInsertUdiskFlag<<"udiskIndexStr = "<< udiskIndexStr;
}

void MediaScreen::sendUdiskInformation()
{
    qDebug()<<"isInsertUdiskFlag = "<< isInsertUdiskFlag<<"udiskIndexStr = "<< udiskIndexStr;
    if (isInsertUdiskFlag)
    {
        emit hasUdiskInsertToComputer(udiskIndexStr);
    }
}

void MediaScreen::init()
{
    engine = new QQmlApplicationEngine();
    if (engine)
    {
        configureSerialer = new ConfigureHolder(engine);
        content = engine->rootContext();
    }
    // 设置Qml的上下文属性，可以直接在qml中引用
#if defined(Q_OS_WIN32)
    nativeFilter = new UsbEventFilter();
    if (nativeFilter)
    {
        if (content)
        {
            content->setContextProperty("UsbHelper", nativeFilter->usbHelper);
            content->setContextProperty("UsbHelper", &mUsbHelper);
            content->setContextProperty("ConfigureSerialer", configureSerialer);
            content->setContextProperty("Ctranslator", CTranslator::instance());
            content->setContextProperty("MediaScreen", this);
            if (engine)
            {
                engine->load(QUrl(QLatin1String("qrc:/main.qml")));
            }
        }
    }
#elif defined(Q_OS_LINUX)
    if (content)
    {
        content->setContextProperty("UsbHelper", &mUsbHelper);
        content->setContextProperty("ConfigureSerialer", configureSerialer);
        content->setContextProperty("Ctranslator", CTranslator::instance());
        content->setContextProperty("MediaScreen", this);
        if (engine)
        {
            engine->load(QUrl(QLatin1String("qrc:/main.qml")));
        }
    }
#endif
    QObject* tmpobj =  engine->rootObjects().value(0);
    if (tmpobj)
    {
        qDebug()<<__PRETTY_FUNCTION__<<"tmpobj = "<<tmpobj;
        QQuickItem* tmpQuick = tmpobj->findChild<QQuickItem *>("parameterSetting");
        if (tmpQuick)
        {
            qDebug()<<__PRETTY_FUNCTION__<<"tmpQuick = "<<tmpQuick;
            qDebug()<<"tmpQuick.width = "<<tmpQuick->width();
            qDebug()<<"tmpQuick.height = "<<tmpQuick->height();
        }
    }
}
