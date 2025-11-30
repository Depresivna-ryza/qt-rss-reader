#ifndef RSS_HPP
#define RSS_HPP

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDebug>
#include <QXmlStreamReader>
#include <QBuffer>
#include <QImageReader>

#include "feed.hpp"
#include "article.hpp"

/*
* @brief RssManager class
* This class is used to fetch the feeds.
* It uses QNetworkAccessManager to download the feeds.
* It uses QXmlStreamReader to parse the feeds.
*/
class RssManager : public QObject
{
    Q_OBJECT

    QNetworkAccessManager *manager;

public:
    explicit RssManager(QObject *parent = nullptr)
        : QObject(parent), manager(new QNetworkAccessManager(this)) {}

    virtual ~RssManager() = default;

    void fetchFeed(const QString &url)
    {
        QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(url)));
        connect(reply, &QNetworkReply::finished, this, [this, reply, url]()
                {
            if (reply->error() == QNetworkReply::NoError) {
                QByteArray data = reply->readAll();
                parseXML(data, url);
            } else {
                emit error(reply->errorString());
                qDebug() << reply->errorString();
            }
            reply->deleteLater(); });
    }

    void parseXML(const QByteArray &xml, const QString &url)
    {
        QXmlStreamReader xmlReader(xml);

        Feed *feed = new Feed();
        feed->lastBuildDate = QDateTime::currentDateTime().toString();
        feed->url = url;
        bool insideItem = false;

        while (!xmlReader.atEnd() && !xmlReader.hasError())
        {
            QXmlStreamReader::TokenType token = xmlReader.readNext();
            if (token != QXmlStreamReader::StartElement)
                continue;

            if (xmlReader.name() == QString("item"))
            {
                feed->articles->addArticle(parseItem(xmlReader));
            }
            else if (xmlReader.name() == QString("title"))
            {
                feed->title = xmlReader.readElementText();
            }
            else if (xmlReader.name() == QString("link"))
            {
                feed->link = xmlReader.readElementText();
            }
            else if (xmlReader.name() == QString("description"))
            {
                feed->description = xmlReader.readElementText();
            }
            else if (xmlReader.name() == QString("image"))
            {
                while (!(xmlReader.tokenType() == QXmlStreamReader::EndElement &&
                         xmlReader.name() == QString("image")))
                {
                    if (xmlReader.name() == QString("url"))
                    {
                        QString url = xmlReader.readElementText();
                        feed->image = downloadImage(url);
                    }
                    xmlReader.readNext();
                }
            }
        }

        if (xmlReader.hasError())
        {
            emit error(xmlReader.errorString());
            return;
        }

        emit fetched(feed);
    }

    Article parseItem(QXmlStreamReader &xmlReader)
    {
        Article article;
        while (!xmlReader.atEnd() && !xmlReader.hasError())
        {
            QXmlStreamReader::TokenType token = xmlReader.readNext();
            if (token == QXmlStreamReader::StartElement)
            {
                if (xmlReader.name() == QString("title"))
                {
                    article.title = xmlReader.readElementText();
                }
                else if (xmlReader.name() == QString("link"))
                {
                    article.link = xmlReader.readElementText();
                }
                else if (xmlReader.name() == QString("description"))
                {
                    article.description = xmlReader.readElementText();
                }
                else if (xmlReader.name() == QString("pubDate"))
                {
                    article.pubDate = xmlReader.readElementText();
                }
                else if (xmlReader.name() == QString("author"))
                {
                    article.author = xmlReader.readElementText();
                }
                else if (xmlReader.name() == QString("enclosure"))
                {
                    article.enclosure = xmlReader.readElementText();
                }
            }
            else if (token == QXmlStreamReader::EndElement)
            {
                if (xmlReader.name() == QString("item"))
                {
                    break;
                }
            }
        }
        return article;
    }
    QImage downloadImage(const QUrl &url)
    {
        QNetworkAccessManager manager;
        QNetworkRequest request(url);
        QNetworkReply *reply = manager.get(request);

        QEventLoop loop;
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        if (reply->error() != QNetworkReply::NoError)
        {
            delete reply;
            return QImage(); // Return an empty image in case of error
        }

        QByteArray imageData = reply->readAll();
        delete reply;

        QBuffer buffer(&imageData);
        buffer.open(QIODevice::ReadOnly);
        QImageReader reader(&buffer, "PNG");
        QImage image = reader.read();

        return image;
    }

signals:
    void fetched(Feed* feed);
    void error(const QString &errorString);
};

#endif // RSS_HPP