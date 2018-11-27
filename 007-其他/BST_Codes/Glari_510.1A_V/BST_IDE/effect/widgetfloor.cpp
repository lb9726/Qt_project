#include "widgetfloor.h"
#include "effectbase.h"

WidgetFloor::WidgetFloor(QWidget *parent) :
    WidgetBase(parent)
{
    m_HundredWidget = 0;
    m_TenWidget = 0;
    m_SingleWidget = 0;
}

WidgetFloor::~WidgetFloor()
{

}

void WidgetFloor::InitFloor(WidgetBase *pHundred, WidgetBase *pTen, WidgetBase *pSingle)
{
    m_HundredWidget = pHundred;
    m_TenWidget = pTen;
    m_SingleWidget = pSingle;
}

void WidgetFloor::InitRC(QString pRcPath, bool pInititial)
{
    WidgetBase::InitRC(pRcPath, pInititial);
    if(m_HundredWidget)
    {
        m_HundredWidget->m_FileList = m_FileList;
        m_HundredWidget->m_ImgList = m_ImgList;
    }
    if(m_TenWidget)
    {
        m_TenWidget->m_FileList = m_FileList;
        m_TenWidget->m_ImgList = m_ImgList;
    }
    if(m_SingleWidget)
    {
        m_SingleWidget->m_FileList = m_FileList;
        m_SingleWidget->m_ImgList = m_ImgList;
    }
}

void WidgetFloor::slot_ExecOperate(AREA_OPERATE pOperate, QVariant pPara)
{
    switch(pOperate)
    {
        case OPERATE_CHANGE:
        {
            if(m_HundredWidget || m_TenWidget || m_SingleWidget)
            {
                FLOOR_INFO tmpFloor = pPara.toInt();
                if(m_HundredWidget && (tmpFloor.mHundredBits != m_OldFloor.mHundredBits))
                    m_HundredWidget->slot_ExecOperate(OPERATE_CHANGE, tmpFloor.mHundredBits);
                if(m_TenWidget && (tmpFloor.mTenBits != m_OldFloor.mTenBits))
                    m_TenWidget->slot_ExecOperate(OPERATE_CHANGE, tmpFloor.mTenBits);
                if(m_SingleWidget && (tmpFloor.mSingleBits != m_OldFloor.mSingleBits))
                    m_SingleWidget->slot_ExecOperate(OPERATE_CHANGE, tmpFloor.mSingleBits);
                m_OldFloor = tmpFloor;
            }
            else
            {
                FLOOR_INFO tmpFloor = pPara.toInt();
                WidgetBase::slot_ExecOperate(pOperate, tmpFloor.toIndex());
            }
            break;
        }
        default:
            break;
    }
}

