#ifndef XMLPARSE_H
#define XMLPARSE_H

#include <QObject>
#include <QRect>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QStringList>
#include <QDomDocument>


class XmlParse : public QObject, public QDomDocument
{
    Q_OBJECT
public:
    XmlParse();
    XmlParse(QString fileName);
    ~XmlParse();


    static const QString XmlHead;
    static const QString XmlSuffix;

    void SetForcesave(bool pEnable) {m_ForceSave = pEnable;}
    //根据路径查找
    bool getItemElement(QString itemPath);
    //以元素itemDomElement为根路径进行查找
    bool getItemElement(QDomElement itemDomElement, QString itemPath);
    //模糊查找
    bool searchItemElement(QString itemName);

    //获取元素值 多态
    QString getItemElementValue();
    QString getItemElementValue(QString itemPath);

    //遍历属性
    QList<QDomAttr> getItemElementAttrs(QDomElement itemDomElement);
    QList<QDomAttr> getItemElementAttrs();
    QList<QDomAttr> getItemElementAttrs(QString itemPath);

    //创建元素节点
    bool createItemElement(QString itemPath);
    bool createItemElement(QDomElement pParElement, QString pName, QString pValue);

    //保存某一个元素为XML文本
    bool saveItemElement(QDomElement itemDomElement, QString& pContent);     //保存到pContent中
    bool saveItemElement(QString &pContent);
    bool saveItemElement(QDomElement itemDomElement, QIODevice *pDevice);   //保存到IODevice中
    bool saveItemElement(QIODevice *pDevice);

    bool modifyItemElement(QString itemPath, QString value);
    bool modifyItemElement(QDomElement itemDomElement, QString value);
    bool setText(QDomElement itemDomElement, QString value);

    bool clearItemElement(QDomElement itemDomElement);
    bool deleteItemElement(QDomElement itemDomElement);

    //获取当前元素所有的子节点
    QDomNodeList getChildNodes()
    {
        return itemElement.childNodes();
    }

    bool writeXml(QIODevice *device);
    bool readXml(QIODevice *device);
    bool openXml();
    bool createXml();
    bool saveXml();
    bool saveXmlAs(QString fileName);
    bool renameXml(QString fileName);

public:
    QString errorStr;
    int errorLine;
    int errorColumn;
    bool m_Valid;
    bool m_IsChanged;
    bool m_ForceSave;
    QString m_FileDir;
    QString m_FileName;
    QDomElement itemElement;
};


#endif // XMLPARSE_H
