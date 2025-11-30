#ifndef ARTICLE_HPP
#define ARTICLE_HPP

#include <QString>
#include <QObject>
#include <QAbstractListModel>


/*
* @brief Article struct
* This struct contains all the information about an article.
*/
struct Article
{
    Article() = default;

    QString title;
    QString link;
    QString description;
    QString pubDate;
    QString author;
    QString enclosure;
};

/*
* @brief ArticleModel class
* This class is a model for the articles.
* It is used by the FeedWidget to display the articles.
*/
class ArticleModel : public QAbstractListModel
{
    Q_OBJECT

    QList<Article> articles;

public:
    ArticleModel(QObject *parent = nullptr) : QAbstractListModel(parent) {}
    virtual ~ArticleModel() = default;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        Q_UNUSED(parent);
        return articles.size();
    }

    Article getArticle(int index)
    {
        return articles[index];
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
    {
        if (!index.isValid() || index.row() >= articles.count())
            return QVariant();

        const Article &article = articles[index.row()];
        switch (role)
        {
        case Qt::DisplayRole:
            return article.title;
        case Qt::UserRole:
            return article.description;
        case Qt::UserRole + 1:
            return article.link;
        case Qt::UserRole + 2:
            return article.pubDate;
        case Qt::UserRole + 3:
            return article.author;
        case Qt::UserRole + 4:
            return article.enclosure;
        case Qt::UserRole + 5:
            return QVariant::fromValue(article);
        default:
            return QVariant();
        }
    }

    void addArticle(const Article &article)
    {
        beginInsertRows(QModelIndex(), articles.count(), articles.count());
        articles.append(article);
        endInsertRows();
    }
};

#endif // ARTICLE_HPP