#include "videolistmodel.h"
#include <QXmlStreamReader>
#include <QVector>
#include <QFile>
#include <QDebug>
typedef QVector<QString> VideoData;

class VideoListModelPrivate
{
public:
    QString m_strXmlFile;
    QString m_strError;
    bool m_bError;
    QHash<int, QByteArray> m_roleNames;
    QVector<VideoData *> m_videos;
public:
    VideoListModelPrivate():m_bError(false)
    {
        int role = Qt::UserRole;
        m_roleNames.insert(role++, "name");
        m_roleNames.insert(role++, "date");
        m_roleNames.insert(role++, "director_tag");
        m_roleNames.insert(role++, "director");
        m_roleNames.insert(role++, "actor_tag");
        m_roleNames.insert(role++, "actor");
        m_roleNames.insert(role++, "rating_tag");
        m_roleNames.insert(role++, "rating");
        m_roleNames.insert(role++, "desc_tag");
        m_roleNames.insert(role++, "desc");
        m_roleNames.insert(role++, "img");
        m_roleNames.insert(role++, "playage");
        m_roleNames.insert(role++, "playtimes");
    }
    ~VideoListModelPrivate()
    {
        clear();
    }
    void load()
    {
        QXmlStreamReader reader;
        QFile file(m_strXmlFile);
        if (!file.exists())
        {
            m_bError = true;
            m_strError = "File Not Found";
            return;
        }
        qDebug()<<__PRETTY_FUNCTION__<<"is reset lines = "<<__LINE__;
        if (!file.open(QFile::ReadOnly))
        {
            m_bError = true;
            m_strError = file.errorString();
            return;
        }
        qDebug()<<__PRETTY_FUNCTION__<<"is reset lines = "<<__LINE__;
        reader.setDevice(&file);
        QStringRef elementName;
        VideoData *video;
        qDebug()<<__PRETTY_FUNCTION__<<"is reset lines = "<<__LINE__;
        while (!reader.atEnd())
        {
            qDebug()<<__PRETTY_FUNCTION__<<"is reset lines = "<<__LINE__;
            reader.readNext();
            if (reader.isStartElement())
            {
                elementName = reader.name();
                if (elementName == "video")
                {
                    video = new VideoData();
                    QXmlStreamAttributes attrs = reader.attributes();
                    video->append(attrs.value("name").toString());
                    video->append(attrs.value("date").toString());
                    qDebug()<<__PRETTY_FUNCTION__<<"is elementName video lines = "<<__LINE__;
                }
                else if ("attr" == elementName)
                {
                    video->append(reader.attributes().value("tag").toString());
                    video->append(reader.readElementText());
                    qDebug()<<__PRETTY_FUNCTION__<<"is elementName attr lines = "<<__LINE__;
                }
                else if ("poster" == elementName)
                {
                    video->append(reader.attributes().value("img").toString());
                    qDebug()<<__PRETTY_FUNCTION__<<"is elementName poster lines = "<<__LINE__;
                }
                else if ("page" == elementName)
                {
                    video->append(reader.attributes().value("link").toString());
                    qDebug()<<__PRETTY_FUNCTION__<<"is elementName page lines = "<<__LINE__;
                }
                else if ("playtimes" == elementName)
                {
                    video->append(reader.readElementText());
                    qDebug()<<__PRETTY_FUNCTION__<<"is elementName playtimes lines = "<<__LINE__;
                }
            }
            else if (reader.isEndDocument())
            {
                elementName = reader.name();
                if (("video") == elementName)
                {
                    m_videos.append(video);
                    video = 0;
                    qDebug()<<__PRETTY_FUNCTION__<<"is elementName video lines = "<<__LINE__;
                }
            }
        }
        file.close();
        if (reader.hasError())
        {
            m_bError = true;
            m_strError = reader.errorString();
        }
        qDebug()<<__PRETTY_FUNCTION__<<"is reset lines = "<<__LINE__;
    }
    void reset()
    {
        m_bError = false;
        m_strError.clear();
        clear();
        qDebug()<<__PRETTY_FUNCTION__<<"is reset lines = "<<__LINE__;
    }
    void clear()
    {
        int count = m_videos.size();
        if (count > 0)
        {
            for (int i = 0; i < count ; ++i)
            {
                delete m_videos.at(i);
            }
            m_videos.clear();
        }
        qDebug()<<__PRETTY_FUNCTION__<<" lines = "<<__LINE__;
    }
};

VideoListModel::VideoListModel(QObject *parent)
  : QAbstractListModel(parent)
  , m_dptr(new VideoListModelPrivate)
{

}

VideoListModel::~VideoListModel()
{
    delete m_dptr;
}

int VideoListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_dptr->m_videos.size();
}

QVariant VideoListModel::data(const QModelIndex &index, int role) const
{
    VideoData *d = m_dptr->m_videos[index.row()];
    return d->at(role - Qt::UserRole);
}

QHash<int, QByteArray> VideoListModel::roleNames() const
{
    qDebug()<<__PRETTY_FUNCTION__<<" lines = "<<__LINE__;
    return m_dptr->m_roleNames;
}

QString VideoListModel::source() const
{
    return m_dptr->m_strXmlFile;
}

void VideoListModel::setSource(const QString &filePath)
{
    m_dptr->m_strXmlFile = filePath;
    reload();
    if (m_dptr->m_bError)
    {
        qDebug()<<__PRETTY_FUNCTION__<<"videoListModel, error - "<<m_dptr->m_strError;
    }
    qDebug()<<__PRETTY_FUNCTION__<<" lines = "<<__LINE__;
}

QString VideoListModel::errorString() const
{
    return m_dptr->m_strError;
}

bool VideoListModel::hashError() const
{
    return m_dptr->m_bError;
}

void VideoListModel::reload()
{
    beginResetModel();
    m_dptr->reset();
    m_dptr->load();
    endResetModel();
}

void VideoListModel::remvoe(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    delete m_dptr->m_videos.takeAt(index);
    endResetModel();
}












