#ifndef FEED_HPP
#define FEED_HPP

#include <QString>
#include <QImage>

#include "article.hpp"

/*
* @brief Feed class
* This class contains all the information about a feed.
*/
class Feed : public QObject
{
    Q_OBJECT
public:
    Feed(QObject *parent = nullptr) : QObject(parent) {}

    QString title;
    QString link;
    QString description;
    QImage image;
    QString lastBuildDate;
    QString url;
    ArticleModel *articles = new ArticleModel(this);
};


/*
* @brief FeedModel class
* This class is a model for the feeds.
* It is used by the FeedListWidget to display the feeds.
*/
class FeedModel : public QAbstractListModel
{
    Q_OBJECT
    QList<Feed *> feeds;

public:
    explicit FeedModel(QObject *parent = nullptr) : QAbstractListModel(parent) {}

    int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        Q_UNUSED(parent);
        return feeds.size();
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
    {
        if (!index.isValid() || index.row() >= feeds.count())
            return QVariant();

        const Feed *feed = feeds[index.row()];
        switch (role)
        {
        case Qt::DisplayRole:
            return feed->title;
        case Qt::UserRole:
            return feed->description;
        case Qt::UserRole + 1:
            return feed->link;
        case Qt::UserRole + 2:
            return feed->lastBuildDate;
        case Qt::UserRole + 3:
            return feed->image;
        case Qt::UserRole + 4:
            return feed->url;
        case Qt::UserRole + 5:
            return QVariant::fromValue(feed->articles);
        case Qt::UserRole + 6:
            return QVariant::fromValue(feed);
        default:
            return QVariant();
        }
    }

    Feed *getFeed(int index)
    {
        return feeds.at(index);
    }

    void addFeed(Feed *feed)
    {
        bool exists = false;

        for (int i = 0; i < feeds.size(); ++i)
        {
            if (feeds[i]->url == feed->url)
            {
                exists = true;
                break;
            }
        }
        if (exists)
            return;

        beginInsertRows(QModelIndex(), feeds.count(), feeds.count());
        feeds.append(feed);
        feed->setParent(this);
        endInsertRows();
    }

    void removeFeed(int index)
    {
        beginRemoveRows(QModelIndex(), index, index);
        feeds.removeAt(index);
        endRemoveRows();
    }

    void clear()
    {
        beginResetModel();
        feeds.clear();
        endResetModel();
    }
};

#endif // FEED_HPP