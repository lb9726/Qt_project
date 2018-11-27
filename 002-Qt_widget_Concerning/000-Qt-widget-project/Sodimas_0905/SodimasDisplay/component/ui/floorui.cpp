#include "floorui.h"
#include <QDebug>

FloorUi::FloorUi(QObject *parent) :
    UiBase(parent)
{
}

FloorUi::FloorUi(ThemeParser *pthemeManager, QDomElement &pElement)
{

}

bool FloorUi::initGeometry(DIRECTION direction, int layout){
    QVariant arg1 = getComRect(direction,layout);
    return invokeQmlMethod("setGeometry",arg1);
}

bool FloorUi::initUi(DIRECTION direction)
{
    Q_UNUSED(direction);
}

bool FloorUi::floorCtrl(int ge,int shi,int bai){
    gePath = getResQmlPath(ge);
    shiPath = getResQmlPath(shi);
    baiPath = getResQmlPath(bai);

//    IDE_TRACE_STR(gePath);
//    IDE_TRACE_STR(shiPath);
//    IDE_TRACE_STR(baiPath);

    ///假设楼层桢中的0x20和0x00不会同时存在
    if(ge == 0x00 && shi == 0x00 && bai == 0x00)
    {
        setFloorVal("","","");
    }else if(ge != 0x00 && shi == 0x00 && bai == 0x00)
    {
        setFloorVal("",gePath,"");
        setOffset(4,2,0);
    }else if(ge == 0x00 && shi != 0x00 && bai == 0x00)
    {
        setFloorVal("",shiPath,"");
        setOffset(4,2,0);
    }else if(ge == 0x00 && shi == 0x00 && bai != 0x00)
    {
        ///IDE_TRACE();
        setFloorVal("",baiPath,"");
        setOffset(4,2,0);
    }else if(ge == 0x00 && shi != 0x00 && bai != 0x00)
    {
        setFloorVal("",shiPath,baiPath);
        setOffset(4,3,1);
    }else if(ge != 0x00 && shi == 0x00 && bai != 0x00)
    {
        setFloorVal(gePath,"",baiPath);
        setOffset(4,2,0);
    }else if(ge != 0x00 && shi != 0x00 && bai == 0x00)
    {
        setFloorVal(gePath,shiPath,"");
        setOffset(3,1,0);
    }else{
        ///IDE_TRACE();
        setFloorVal(gePath,shiPath,baiPath);
        setOffset(4,2,0);
    }



//    gePath = getResQmlPath(ge);
//    shiPath = getResQmlPath(shi);
//    baiPath = getResQmlPath(bai);

//    IDE_TRACE_STR(gePath);
//    IDE_TRACE_STR(shiPath);
//    IDE_TRACE_STR(baiPath);

//    ///假设楼层桢中的0x20和0x00不会同时存在
//    if(ge == 0x00 && shi == 0x00 && bai == 0x00)
//    {
//        setFloorVal("","","");
//    }else if(ge != 0x00 && shi == 0x00 && bai == 0x00)
//    {
//        setFloorVal(gePath,"","");
//        setOffset(2,2,0);
//    }else if(ge == 0x00 && shi != 0x00 && bai == 0x00)
//    {
//        setFloorVal("",shiPath,"");
//        setOffset(4,2,0);
//    }else if(ge == 0x00 && shi == 0x00 && bai != 0x00)
//    {
//        setFloorVal("","",baiPath);
//        setOffset(4,2,2);
//    }else if(ge == 0x00 && shi != 0x00 && bai != 0x00)
//    {
//        setFloorVal("",shiPath,baiPath);
//        setOffset(4,3,1);
//    }else if(ge != 0x00 && shi == 0x00 && bai != 0x00)
//    {
//        setFloorVal(gePath,"",baiPath);
//        setOffset(4,2,0);
//    }else if(ge != 0x00 && shi != 0x00 && bai == 0x00)
//    {
//        setFloorVal(gePath,shiPath,"");
//        setOffset(3,1,0);
//    }else{
//        setFloorVal(gePath,shiPath,baiPath);
//        setOffset(4,2,0);
//    }

    if(ge == 0x20 && shi == 0x20 && bai == 0x20)
    {
        setFloorVal("","","");
    }else if(ge != 0x20 && shi == 0x20 && bai == 0x20)
    {
        setFloorVal(gePath,"","");
        setOffset(4,2,0);
    }else if(ge == 0x20 && shi != 0x20 && bai == 0x20)
    {
        setFloorVal("",shiPath,"");
        setOffset(4,2,0);
    }else if(ge == 0x20 && shi == 0x20 && bai != 0x20)
    {
        setFloorVal("","","");
    }else if(ge == 0x20 && shi != 0x00 && bai != 0x20)
    {
        setFloorVal("","","");
    }else if(ge != 0x20 && shi == 0x20 && bai != 0x20)
    {
        setFloorVal("","","");
    }else if(ge != 0x20 && shi != 0x20 && bai == 0x20)
    {
        setFloorVal("","","");
    }

}

bool FloorUi::setFloorVal(QString gePath,QString shiPath,QString baiPath){
    QVariant arg1 = gePath;
    QVariant arg2 = shiPath;
    QVariant arg3 = baiPath;

    return invokeQmlMethod("setFloor",arg1,arg2,arg3);
}


bool FloorUi::setOffset(int geoffset,int shioffset,int baioffset){
    QVariant offset1 = geoffset;
    QVariant offset2 = shioffset;
    QVariant offset3 = baioffset;

    return invokeQmlMethod("setOffset",offset1,offset2,offset3);
}
