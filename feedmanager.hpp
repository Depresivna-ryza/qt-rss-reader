#ifndef FEEDMANAGER_HPP
#define FEEDMANAGER_HPP

#include <QObject>
#include <QSettings>

#include "feed.hpp"
#include "article.hpp"
#include "rss.hpp"
/*
* @brief FeedManager class
* This class is used to manage the feeds.
* It uses RssManager to fetch the feeds.
* It uses QSettings to save and load the feeds.
*/

class FeedManager : public QObject
{
    Q_OBJECT

    FeedModel feeds;
    RssManager rssManager;
    const QString feedsKey = "rssFeeds";
    const QString feedUrlArray = "feedUrls";

public:
    explicit FeedManager(QObject *parent = nullptr) : QObject(parent)
    {
        connect(&rssManager, &RssManager::fetched, this, [this](Feed *feed)
                { 
                    feeds.addFeed(feed);
                    saveFeeds(); });

        connect(&rssManager, &RssManager::error, this, [this](const QString &errorString)
                { qDebug() << errorString; });

        loadFeeds();
    }

    virtual ~FeedManager() = default;

    FeedModel *getFeeds()
    {
        return &feeds;
    }

    void addFeed(const QString &url)
    {
        rssManager.fetchFeed(url);
    }

    void saveFeeds()
    {
        QSettings settings(feedsKey);

        qDebug() << "saving" << feeds.rowCount() << "feeds";
        qDebug() << settings.allKeys();
        settings.remove(feedUrlArray);
        qDebug() << settings.allKeys();

        settings.beginWriteArray(feedUrlArray);
        for (int i = 0; i < feeds.rowCount(); ++i)
        {
            settings.setArrayIndex(i);
            settings.setValue("url", feeds.data(feeds.index(i), Qt::UserRole + 4));
            qDebug() << feeds.data(feeds.index(i), Qt::UserRole + 4);
        }
        settings.endArray();
    }

    void loadFeeds()
    {
        QSettings settings(feedsKey);

        int size = settings.beginReadArray(feedUrlArray);
        qDebug() << "loading" << size << "feeds";
        for (int i = 0; i < size; ++i)
        {
            settings.setArrayIndex(i);
            QString url = settings.value("url").toString();
            rssManager.fetchFeed(url);
            qDebug() << url;
        }
        settings.endArray();
    }

public slots:
    void removeFeed(int index)
    {
        feeds.removeFeed(index);
        saveFeeds();
    }
};

#endif // FEEDMANAGER_HPP