#include "widget.h"
#include "ui_widget.h"
#include <QFile>
#include <QCoreApplication>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    readXml();
}

bool Widget::readXml()
{
    qDebug()<<__FUNCTION__;
    QString path = QCoreApplication::applicationDirPath();
    qDebug()<<"path = "<< path;
    QFile file(path + "/test.xml");
    if(!file.open(QFile::ReadOnly| QFile::Text)) {
        qDebug()<<"open file error!";
        return false;
    }
    QString errorStr;
    int errorLine;
    int errorColumn;
    QDomDocument doc;
    if(!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
    { //setContent 是将指定的内容指定给QDomDocument 解析，第一参数可以是QByteArray或者是文件名等。
        qDebug()<<"return false";
        return false;
    }
    file.close();
//    QDomNodeList list = doc.childNodes();
//    qDebug()<<"list.count()="<< list.count();
//    qDebug()<<"firstchild = "<<doc.firstChild().firstChildElement().toText();
//    for(int i = 0; i<list.count(); i++)
//    {
//        QDomNode  node = list.at(i);
//        if (node.hasChildNodes())
//        {
//            QDomNodeList childlist = node.childNodes();
//            for(int i = 0; i<childlist.count(); i++)
//            {
//                QDomNode  node1  =childlist.at(i);
//                qDebug()<<"node1 name is"<<node1.nodeName();
//                qDebug()<<"node1 type is"<<node1.nodeType();
//            }
//        }
//        else {
//            qDebug()<<"node name is"<<node.nodeName();
//            qDebug()<<"node type is"<<node.nodeType();
//        }
//    }
    QDomElement docElem = doc.documentElement();
    qDebug()<<docElem.tagName();
    QDomNode n = docElem.firstChild();
    qDebug()<<n.toElement().tagName();
    int i = 0;
    while(!n.isNull())
    {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull())
        {
            qDebug()<<e.tagName() << ":" << e.text();

        }
        qDebug()<<"i = "<< ++i;
        n = n.nextSibling();
    }
    return true;
}

void Widget::outPutAllXml(QDomNode doc)
{
//    qDebug()<<__FUNCTION__;
//    QDomNodeList list = doc.childNodes();
//    qDebug()<<"list.count()="<< list.count();
//    if (node.hasChildNodes())
//    {
//        for(int i = 0; i<list.count(); i++)
//        {
//            qDebug()<<"node name is"<<node.nodeName();
//            qDebug()<<"node type is"<<node.nodeType();
//        }
//    }
}

Widget::~Widget()
{
    delete ui;
}
