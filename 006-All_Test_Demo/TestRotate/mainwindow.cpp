#include "mainwindow.h"

Uiloader *gUiLoader = 0;

Uiloader::Uiloader(QObject *parent)
    :QUiLoader(parent)
{
    gUiLoader = this;

    mView = 0;

    m_themeParser = 0;
    m_themeLayout = 0;
    m_themeRotate = 0;
    m_themeDirection = Horizontal;

    mDesktop = 0;
    mArrow = 0;
    mFloor = 0;
    mIcon = 0;
    mIsShowLogo = true;
    mScrollSpeed = 0;

    mAutoTestCount = 20;
    connect(&mAutoTestTimer, SIGNAL(timeout()), this, SLOT(slot_AutoTest()));
    mAutoTestTimer.setInterval(600);
}

void Uiloader::slot_ShowFuncLogo(int pFuncLogoIndex, int pSoundNumber)
{
    IDE_TRACE_INT(pFuncLogoIndex);
    IDE_TRACE_INT(pSoundNumber);

    //显示ICON的代码
    if(mIsShowLogo)
    {
        if(mFloor)
            mFloor->setVisible(false);
        if(mIcon)
        {
            mIcon->setVisible(true);
            mIcon->setIcon(pFuncLogoIndex);
        }
    }
}

void Uiloader::slot_FloorArrive(int pGe, int pShi, int pBai, int pSoundNumber)
{
    IDE_TRACE_INT(pGe);
    IDE_TRACE_INT(pShi);
    IDE_TRACE_INT(pBai);
    IDE_TRACE_INT(pSoundNumber);
    if(mFloor)
    {
        //显示Floor的代码
        if(mIcon)
            mIcon->setVisible(false);
        if(mFloor)
        {
            mFloor->setVisible(true);
            mFloor->setFloor(pGe, pShi, pBai);
        }
    }
}

void Uiloader::slot_ArrowAction(bool pArrows_FLM, bool pArrows_FLD, bool pArrows_SCROL)
{
    if(mArrow)
    {
        if(pArrows_FLM == true && pArrows_FLD == false && pArrows_SCROL == false)
        {
            mArrow->setArrowType(UiArrow::UpArrow);
        }
        else if(pArrows_FLM == false && pArrows_FLD == true && pArrows_SCROL == false)
        {
            mArrow->setArrowType(UiArrow::DownArrow);
        }
        else if(pArrows_FLM == true && pArrows_FLD == false && pArrows_SCROL == true)
        {
            mArrow->setArrowType(UiArrow::UpScrollArrow, mScrollSpeed);
        }
        else if(pArrows_FLM == false && pArrows_FLD == true && pArrows_SCROL == true)
        {
            mArrow->setArrowType(UiArrow::DownScrollArrow, mScrollSpeed);
        }
        else if(pArrows_FLM == true && pArrows_FLD == true)
        {
            mArrow->setArrowType(UiArrow::UpDownArrow);
        }
        else if(pArrows_FLM == false && pArrows_FLD == false)
        {
            mArrow->setArrowType(UiArrow::NoArrow);
        }
    }
}

void Uiloader::slot_Orient(int pOrientation) //sdo旋转横竖屏
{
    IDE_TRACE_INT(pOrientation);
}

void Uiloader::slot_AutoTest()
{
    mAutoTestCount++;

    if(mAutoTestCount < 5)
    {
        SetLayout(0);
        SetRotate(0);

        //小屏模式
        if(mIcon)
        {
            mIcon->setBigSrnMode(false);
        }

        //显示Floor的代码
        if(mIcon)
            mIcon->setVisible(false);
        if(mFloor)
        {
            mFloor->setVisible(true);
            mFloor->setFloor(mAutoTestCount%10, mAutoTestCount/10, 0);  //GE SHI BAI
        }

        //显示Arroow的代码
        if(mArrow)
        {
            mArrow->show();
            mArrow->setArrowType(UiArrow::UpScrollArrow, 5);
        }
    }
    else if(mAutoTestCount < 10)
    {
        SetLayout(0);
        SetRotate(270);
    }
    else if(mAutoTestCount < 20)
    {
        SetLayout(0);
        SetRotate(90);
    }
    else if(mAutoTestCount < 30)
    {
        SetLayout(0);
        SetRotate(180);
    }
    else if(mAutoTestCount < 40)
    {
        SetLayout(0);
        SetRotate(270);
    }
    else if(mAutoTestCount < 49)
    {
        SetLayout(0);
        SetRotate(0);

        //小屏模式
        if(mIcon)
        {
            mIcon->setBigSrnMode(false);
        }

        //显示ICON的代码
        if(mFloor)
            mFloor->setVisible(false);
        if(mIcon)
        {
            mIcon->setVisible(true);
            mIcon->setIcon(mAutoTestCount);
        }

        //显示Arroow的代码
        if(mArrow)
        {
            mArrow->show();
            mArrow->setArrowType(UiArrow::DownScrollArrow, 2);
        }
    }
    else if(mAutoTestCount < 55)
    {
        SetLayout(0);
        SetRotate(0);

        //大屏模式
        if(mIcon)
        {
            mIcon->setBigSrnMode(true);
            QString tmpText = QString("%1 %1 %1 %1 %1 %1 %1").arg(mAutoTestCount);
            mIcon->setText(tmpText, tmpText, tmpText, tmpText);
        }
    }
    else if(mAutoTestCount < 60)
    {
        SetLayout(0);
        SetRotate(90);

        //大屏模式
        if(mIcon)
        {
            mIcon->setBigSrnMode(true);
            QString tmpText = QString("%1 %1 %1 %1 %1 %1 %1").arg(mAutoTestCount);
            mIcon->setText(tmpText, tmpText, tmpText, tmpText);
        }
    }
    else if(mAutoTestCount < 65)
    {
        SetLayout(0);
        SetRotate(90);

        //小屏模式
        if(mIcon)
        {
            mIcon->setBigSrnMode(false);
        }

        //显示Arroow的代码
        if(mArrow)
        {
            mArrow->show();
            mArrow->setArrowType(UiArrow::UpScrollArrow, 1);
        }

        //显示Floor的代码
        if(mIcon)
            mIcon->setVisible(false);
        if(mFloor)
        {
            mFloor->setVisible(true);
            mFloor->setFloor(mAutoTestCount%10, mAutoTestCount/10, 0);  //GE SHI BAI
        }
    }
    else if(mAutoTestCount < 70)
    {
        SetLayout(1);
        SetRotate(90);

        //小屏模式
        if(mIcon)
        {
            mIcon->setBigSrnMode(false);
        }

        //显示Arroow的代码
        if(mArrow)
        {
            mArrow->show();
            mArrow->setArrowType(UiArrow::UpScrollArrow, 1);
        }

        //显示Floor的代码
        if(mIcon)
            mIcon->setVisible(false);
        if(mFloor)
        {
            mFloor->setVisible(true);
            mFloor->setFloor(mAutoTestCount%10, mAutoTestCount/10, 0);  //GE SHI BAI
        }
    }
    else if(mAutoTestCount >= 70)
    {
        SetRotate(0);
        mAutoTestCount = 0;
    }
}


//void Uiloader::slot_AutoTest()
//{
//    mAutoTestCount ++;

//    if(mFloor)
//    {
//        mFloor->setVisible(true);
//        mFloor->setFloor(mAutoTestCount%10, mAutoTestCount/10, 0); //GE SHI BAI
//        mFloor->setTimeAndDest("01/01/10 10:41", "Destination: --");
//    }
//    if(mArrow)
//    {
//        mArrow->show();
//        mArrow->setArrowType(UiArrow::UpScrollArrow, 3);
//    }
//    if (mAutoTestCount >= 10)
//    {
//        if (mAutoTestCount > 20)
//        {
//            mAutoTestCount = 0;
//        }
//        mView->resetCachedContent();
//        SetRotate(90);
//    }
//}

//void Uiloader::slot_AutoTest()
//{
//    mAutoTestCount++;

//    if(mAutoTestCount < 40)
//    {
//        SetLayout(0);
//        SetRotate(0);
//        //小屏模式
//        if(mIcon)
//        {
//            mIcon->setBigSrnMode(false);
//        }
//        //显示Floor的代码
//        if(mIcon)
//            mIcon->setVisible(false);
//        if(mFloor)
//        {
//            mFloor->setVisible(true);
//            mFloor->setFloor(mAutoTestCount%10, mAutoTestCount/10, 0); //GE SHI BAI
//        }
//        //显示Arroow的代码
//        if(mArrow)
//        {
//            mArrow->show();
//            mArrow->setArrowType(UiArrow::UpScrollArrow, 5);
//        }
//    }
//    else if(mAutoTestCount < 49)
//    {
//        SetLayout(0);
//        SetRotate(0);
//        //小屏模式
//        if(mIcon)
//        {
//            mIcon->setBigSrnMode(false);
//        }
//        //显示ICON的代码
//        if(mFloor)
//            mFloor->setVisible(false);
//        if(mIcon)
//        {
//            mIcon->setVisible(true);
//            mIcon->setIcon(mAutoTestCount);
//        }
//        //显示Arroow的代码
//        if(mArrow)
//        {
//            mArrow->show();
//            mArrow->setArrowType(UiArrow::DownScrollArrow, 2);
//        }
//    }
//    else if(mAutoTestCount < 55)
//    {
//        SetLayout(0);
//        SetRotate(0);
//        //大屏模式
//        if(mIcon)
//        {
//            mIcon->setBigSrnMode(true);
//            QString tmpText = QString("%1 %1 %1 %1 %1 %1 %1").arg(mAutoTestCount);
//            mIcon->setText(tmpText, tmpText, tmpText, tmpText);
//        }
//    }
//    else if(mAutoTestCount < 60)
//    {
//        SetLayout(0);
//        SetRotate(90);
//        //大屏模式
//        if(mIcon)
//        {
//            mIcon->setBigSrnMode(true);
//            QString tmpText = QString("%1 %1 %1 %1 %1 %1 %1").arg(mAutoTestCount);
//            mIcon->setText(tmpText, tmpText, tmpText, tmpText);
//        }
//    }
//    else if(mAutoTestCount < 65)
//    {
//        SetLayout(0);
//        SetRotate(90);
//        //小屏模式
//        if(mIcon)
//        {
//            mIcon->setBigSrnMode(false);
//        }
//        //显示Arroow的代码
//        if(mArrow)
//        {
//            mArrow->show();
//            mArrow->setArrowType(UiArrow::UpScrollArrow, 1);
//        }
//        //显示Floor的代码
//        if(mIcon)
//            mIcon->setVisible(false);
//        if(mFloor)
//        {
//            mFloor->setVisible(true);
//            mFloor->setFloor(mAutoTestCount%10, mAutoTestCount/10, 0); //GE SHI BAI
//        }
//    }
//    else if(mAutoTestCount < 70)
//    {
//        SetLayout(1);
//        SetRotate(90);
//        //小屏模式
//        if(mIcon)
//        {
//            mIcon->setBigSrnMode(false);
//        }
//        //显示Arroow的代码
//        if(mArrow)
//        {
//            mArrow->show();
//            mArrow->setArrowType(UiArrow::UpScrollArrow, 1);
//        }
//        //显示Floor的代码
//        if(mIcon)
//            mIcon->setVisible(false);
//        if(mFloor)
//        {
//            mFloor->setVisible(true);
//            mFloor->setFloor(mAutoTestCount%10, mAutoTestCount/10, 0); //GE SHI BAI
//        }
//    }
//    else if(mAutoTestCount >= 70)
//    {
//        SetLayout(0);
//        SetRotate(0);
//        mAutoTestCount = 0;
//    }
//}

//这个不是类函数
bool CopyFileContent(QString srcFile, QString dstFile)
{
    //>@如果路径相同，则不进行拷贝
    if(srcFile.compare(dstFile) == 0)
        return true;

    QFileInfo tmpSrcFileInfo(srcFile);
    if(!tmpSrcFileInfo.isFile())
    {
        return false;
    }

    if (dstFile.isEmpty())
        return false;
    dstFile.replace("\\", "/");
    if(dstFile.endsWith('/'))
        return false;
    int index = dstFile.lastIndexOf('/');
    if(index < 0)
        return false;
    QString tmpDstPath = dstFile.mid(0, index + 1);
    if(tmpDstPath.isEmpty())
    {
        return false;
    }
    QDir tmpDstDir;
    if(!tmpDstDir.mkpath(tmpDstPath))
    {
        return false;
    }
    bool flag = QFile::copy(srcFile, dstFile);
    return flag;
}

QWidget* Uiloader::createWidget(const QString &className, QWidget *parent, const QString &name)
{
    if(!className.compare("UiArrow", Qt::CaseInsensitive))
    {
        UiArrow* tmpArrow = new UiArrow(parent);
        tmpArrow->setObjectName(name);
        mArrow = tmpArrow;
        return tmpArrow;
    }
    if(!className.compare("UiFloor", Qt::CaseInsensitive))
    {
        UiFloor* tmpFloor = new UiFloor(parent);
        tmpFloor->setObjectName(name);
        mFloor = tmpFloor;
        return tmpFloor;
    }
//    if(!className.compare("UiClock", Qt::CaseInsensitive))
//    {
//        UiClock* tmpClock = new UiClock;
//        tmpClock->setObjectName(name);
//        tmpClock->Init();
//        return tmpClock;
//    }
    if(!className.compare("UiDesktop", Qt::CaseInsensitive))
    {
        UiDesktop* tmpDesktop = new UiDesktop(parent);
        tmpDesktop->setObjectName(name);
        mDesktop = tmpDesktop;
        return tmpDesktop;
    }
    if(!className.compare("UiIcon", Qt::CaseInsensitive))
    {
        UiIcon* tmpIcon = new UiIcon(parent);
        tmpIcon->setObjectName(name);
        mIcon = tmpIcon;
        return tmpIcon;
    }
//    if(!className.compare("UiVideo", Qt::CaseInsensitive))
//    {
//        UiVideo* tmpVideo = new UiVideo;
//        tmpVideo->setObjectName(name);
//        tmpVideo->Init();
//        return tmpVideo;
//    }
    return QUiLoader::createWidget(className, parent, name);
}

void Uiloader::InitComponent(COM_TYPE pType, QDomElement pElement)
{
    if(pType == COM_DESKTOP && mDesktop)
    {
        mDesktop->Init(pElement);
    }
    else if(pType == COM_FLOOR && mFloor)
    {
        mFloor->Init(pElement);
    }
    else if(pType == COM_FUNCTION && mIcon)
    {
        mIcon->Init(pElement);
    }
    else if(pType == COM_ARROW && mArrow)
    {
        mArrow->Init(pElement);
    }
}

void Uiloader::SetLayout(int pLayout)
{
    if(m_themeLayout != pLayout)
    {
        m_themeLayout = pLayout;

        //重新初始化所有控件
        QList<COM_TYPE> tmpComTypeList = m_themeParser->m_ComElementGroup.keys();
        int count = tmpComTypeList.count();
        for(int i=0; i<count; i++)
        {
            COM_TYPE tmpType = tmpComTypeList.at(i);

            QDomElement tmpElement = m_themeParser->m_ComElementGroup.value(tmpType);
            if(tmpElement.isNull())
                continue;
            InitComponent(tmpType, tmpElement);
        }
    }
}

void Uiloader::SetRotate(int pRotate)
{
    if(m_themeRotate != pRotate)
    {
        if(pRotate == 0 || pRotate == 180)
        {
            m_themeDirection = Horizontal;
        }
        else
        {
            m_themeDirection = Vertical;
        }

        mView->rotate(pRotate - m_themeRotate);
        m_themeRotate = pRotate;

        //重新初始化所有控件
        QList<COM_TYPE> tmpComTypeList = m_themeParser->m_ComElementGroup.keys();
        int count = tmpComTypeList.count();
        for(int i=0; i<count; i++)
        {
            COM_TYPE tmpType = tmpComTypeList.at(i);

            QDomElement tmpElement = m_themeParser->m_ComElementGroup.value(tmpType);
            if(tmpElement.isNull())
                continue;
            InitComponent(tmpType, tmpElement);
        }
    }
}

QWidget *Uiloader::Init(QString pUiPath)
{
    QWidget* tmpWidget = 0;
    QString tmpRccStr = pUiPath + "Theme.rcc";
    QResource::registerResource(tmpRccStr);

    QString tmpUiStr = pUiPath + "Test.ui";
    QFile tmpFile(tmpUiStr);
    if(!tmpFile.open(QFile::ReadOnly))
        return tmpWidget;
    tmpWidget = load(&tmpFile);

    m_themeParser = new ThemeParser(pUiPath + "theme.xml");
    IDE_TRACE_STR(QString("after theme.xml"));
    if(m_themeParser->openUi())
    {
        QList<COM_TYPE> tmpComTypeList = m_themeParser->m_ComElementGroup.keys();
        int count = tmpComTypeList.count();
        for(int i=0; i<count; i++)
        {   IDE_TRACE();
            COM_TYPE tmpType = tmpComTypeList.at(i);

            QDomElement tmpElement = m_themeParser->m_ComElementGroup.value(tmpType);
            if(tmpElement.isNull())
                continue;
            InitComponent(tmpType, tmpElement);
        }
    }

    mAutoTestTimer.start();
    return tmpWidget;
}
