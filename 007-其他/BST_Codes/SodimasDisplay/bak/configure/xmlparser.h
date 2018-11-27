#ifndef XMLPARSER_H
#define XMLPARSER_H
#include <QtXml>
//#include <QSvgRenderer>

#include "global/filefunction.h"

class DomDocument : public QDomDocument
{
public:
    QString m_FileDir;
    QString m_FileName;
};

class XmlParser : public QObject, public DomDocument
{
    Q_OBJECT
public:
    XmlParser();
    XmlParser(QString fileName);
    ~XmlParser();

    void RemoveAttribute(QDomElement pElement, QString pName);
    void SetAttribute(QDomElement pElement, QString pName, QString pValue);

    //>@根据路径查找
    bool GetItemElement(QString itemPath);
    //>@以元素itemDomElement为根路径进行查找
    bool GetItemElement(QDomElement itemDomElement, QString itemPath);

    //>@模糊查找
    bool SearchItemElement(QString itemName);

    //>@获取元素值 多态
    QString GetItemElementValue();
    QString GetItemElementValue(QString itemPath);

    //>@遍历属性
    static QList<QDomAttr> GetItemElementAttrs(QDomElement itemDomElement);
    QList<QDomAttr> GetItemElementAttrs();
    QList<QDomAttr> GetItemElementAttrs(QString itemPath);

    //创建元素节点
    bool CreateItemElement(QString itemPath);
    bool CreateItemElement(QDomElement pParElement, QString pName, QString pValue="");

    //>@保存某一个元素为XML文本
    bool SaveItemElement(QDomElement itemDomElement, QString& pContent);     //>@保存到pContent中
    bool SaveItemElement(QString &pContent);
    bool SaveItemElement(QDomElement itemDomElement, QIODevice *pDevice);   //>@保存到IODevice中
    bool SaveItemElement(QIODevice *pDevice);

    bool ModifyItemElement(QString itemPath, QString value);
    bool ModifyItemElement(QDomElement itemDomElement, QString value);
    bool SetText(QDomElement itemDomElement, QString value);

    bool ClearItemElement(QDomElement itemDomElement);
    bool DeleteItemElement(QDomElement itemDomElement);

    //获取当前元素所有的子节点
    QDomNodeList GetChildNodes()
    {
        return itemElement.childNodes();
    }

    bool write(QIODevice *device);
    bool read(QIODevice *device);
    bool Open();
    bool New();
    bool Save();
    bool SaveAs(QString fileName);
    bool Rename(QString fileName);

public:
    static const QString XmlHead;
    static const QString XmlSuffix;

    QString errorStr;
    int errorLine;
    int errorColumn;

    bool m_Valid;

    bool m_IsChanged;
    bool m_ForceSave;

    void SetForcesave(bool pEnable) {m_ForceSave = pEnable;}

    QDomElement itemElement;
    bool CreateElementPath(QDomElement itemDomElement, QString itemPath);
};

#endif // XMLPARSER_H

