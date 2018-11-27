#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    init();
    m_mdpParser->modifyItemElement("/configure/physical/lcd/rotation", "123");
    m_mdpParser->saveXml();
//    m_themeParser->saveXmlAs(mRunPath + "theme2.xml");
//    m_mdpParser->saveMdp();
//    m_themeParser->saveXml();
//    QString mProtocolType = m_mdpParser->getProtocolType();
//    qDebug()<<m_mdpParser->getPassword();
//    qDebug()<<mProtocolType;
//    m_mdpParser->modifyItemElement("/protocol", "saf");
//    m_mdpParser->saveXmlAs(mRunPath + "device.xml");
/*
    if (!QString::compare("canopen", mProtocolType, Qt::CaseInsensitive))
    {
        qDebug()<<"mProtocolType = "<<mProtocolType;
    }
    else if (!QString::compare("saf", mProtocolType, Qt::CaseInsensitive))
    {
        qDebug()<<"mProtocolType = "<<mProtocolType;
    }
    else
    {
        IDE_TRACE_STR(QString("protocolType is not legal"));
    }

    // get ip
    if (m_mdpParser)
    {
        QDomElement tmpLancElement = m_mdpParser->m_PHElementGroup.value(P_LANC);
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"mTcpPort = ";
        if(!tmpLancElement.isNull())
        {
            QDomElement tmpElement = tmpLancElement.firstChildElement("tcp");
            if(!tmpElement.isNull())
            {
                int mTcpPort = tmpElement.text().toInt();
                qDebug()<<"mTcpPort = "<<"lines = "<<__LINE__<<mTcpPort;
//                mWiFi = new WifiCommunication(mTcpPort);
//                initWifi();
            }
        }
    }
*/
}

Widget::~Widget()
{
    delete ui;
}

void Widget::callOpenMdp()
{
    if (m_mdpParser->openMdp())
    {
        QDomElement tmpPlayerElement = m_mdpParser->m_PHElementGroup.value(P_Player);
        if (!tmpPlayerElement.isNull())
        {
            QDomElement tmpElement = tmpPlayerElement.firstChildElement("admusicvol");
            if (!tmpElement.isNull())
            {
                m_admusicvol = tmpElement.text().toInt();
            }
            if (m_admusicvol >= 100)
            {
                m_admusicvol = 99;
            }
            else if (m_admusicvol < 0)
            {
                m_admusicvol = 0;
            }
            tmpElement = tmpPlayerElement.firstChildElement("beepvol");
            if (!tmpElement.isNull())
            {
                m_beepvol = tmpElement.text().toInt();
            }
            tmpElement = tmpPlayerElement.firstChildElement("liftarvvol");
            if (!tmpElement.isNull())
            {
                m_liftarvvol = tmpElement.text().toInt();
            }
            tmpElement = tmpPlayerElement.firstChildElement("liftflrvol");
            if (!tmpElement.isNull())
            {
                m_liftflrvol = tmpElement.text().toInt();
            }
        }

        QDomElement tmpLedElement = m_mdpParser->m_PHElementGroup.value(P_LED);
        if (!tmpLedElement.isNull())
        {
            QDomElement tmpElement = tmpLedElement.firstChildElement("normal");
            if (!tmpElement.isNull())
            {
                m_lednormalFreq = tmpElement.text();
            }
            tmpElement = tmpLedElement.firstChildElement("busy");
            if (!tmpElement.isNull())
            {
                m_ledbusyFreq = tmpElement.text();
            }
            tmpElement = tmpLedElement.firstChildElement("error");
            if (!tmpElement.isNull())
            {
                m_lederrorFreq = tmpElement.text();
            }
        }

        QDomElement tmpLcdElement = m_mdpParser->m_PHElementGroup.value(P_LCD);
        if (!tmpLcdElement.isNull())
        {
            QDomElement tmpElement = tmpLcdElement.firstChildElement("rotation");
            if (!tmpElement.isNull())
            {
                m_rotation = tmpElement.text().toInt() + 180;
            }
            tmpElement = tmpLcdElement.firstChildElement("luminance");
            if (!tmpElement.isNull())
            {
                m_luminance = tmpElement.text().toInt();
            }
            tmpElement = tmpLcdElement.firstChildElement("standby");
            if (!tmpElement.isNull())
            {
                m_standby = tmpElement.text().toInt();
            }
            tmpElement = tmpLcdElement.firstChildElement("standbyluminance");
            if (!tmpElement.isNull())
            {
                m_standbyluminance = tmpElement.text().toInt();
            }
            tmpElement = tmpLcdElement.firstChildElement("screensave");
            if (!tmpElement.isNull())
            {
                m_screensave = tmpElement.text().toInt();
            }
            tmpElement = tmpLcdElement.firstChildElement("screensavehold");
            if (!tmpElement.isNull())
            {
                m_screensavehold = tmpElement.text().toInt();
            }
        }

        QDomElement tmpComElement = m_mdpParser->m_PHElementGroup.value(P_COM);
        if (!tmpComElement.isNull())
        {
            QDomElement tmpElement = tmpComElement.firstChildElement("autotest");
            if (!tmpElement.isNull())
            {
                m_autotest = (bool)(!tmpElement.text().compare("true"));
            }
            tmpElement = tmpComElement.firstChildElement("testfreq");
            if (!tmpElement.isNull())
            {
                m_testfreq = tmpElement.text().toInt();
            }

            tmpElement = tmpComElement.firstChildElement("safOpenMessage");
            IDE_TRACE_STR(tmpElement.text());
            if (!tmpElement.isNull())
            {
                m_safOpenningMessage = (bool)(!tmpElement.text().compare("true"));
            }
        }

        QDomElement tmpThemeElement = m_mdpParser->m_LGElementGroup.value(L_ThemeManager);
        if (!tmpThemeElement.isNull())
        {
            QDomElement tmpElement = tmpThemeElement.firstChildElement("layout");
            if (!tmpElement.isNull())
            {
                m_layout = tmpElement.text().toInt();
            }
        }
    }
}

void Widget::init()
{
    IDE_TRACE();
    getRunPath();
    m_mdpParser = new MdpParser(mRunPath + "device.xml");
    if (m_mdpParser)
    {
        callOpenMdp();
    }
    m_themeParser = new ThemeParser(mRunPath + "theme.xml") ;
    if (m_themeParser)
    {
        if(m_themeParser->openUi())
        {
            QList<COM_TYPE> tmpComTypeList = m_themeParser->m_ComElementGroup.keys();
            int count = tmpComTypeList.count();
            for(int i = 0; i < count; ++i)
            {
                COM_TYPE tmpType = tmpComTypeList.at(i);
                QDomElement tmpElement = m_themeParser->m_ComElementGroup.value(tmpType);
                if(tmpElement.isNull())
                {
                    continue;
                }
//                InitComponent(tmpType, tmpElement);
            }
        }
    }
}

void Widget::getRunPath()
{
    mRunPath = QCoreApplication::applicationDirPath();
    if (!mRunPath.endsWith("/"))
    {
        mRunPath.append("/");
    }
    IDE_TRACE_STR(mRunPath);
}
