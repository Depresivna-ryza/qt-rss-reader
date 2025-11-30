#ifndef FEEDWIDGET_HPP
#define FEEDWIDGET_HPP

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPixmap>
#include <QModelIndex>

#include "feed.hpp"
#include "feedtopwidget.hpp"
#include "feedbottomwidget.hpp"

/*
* @brief FeedWidget class
* This class is the right part of the Client.
* It contains the FeedTopWidget and the FeedBottomWidget.
*/
class FeedWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FeedWidget(QWidget *parent = nullptr)
    {
        feedTopWidget = new FeedTopWidget(this);

        feedBottomWidget = new FeedBottomWidget(this);

        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->addWidget(feedTopWidget);
        mainLayout->addWidget(feedBottomWidget, 1);
        mainLayout->setContentsMargins(0, 0, 0, 0);
        setLayout(mainLayout);
        connect(feedTopWidget, &FeedTopWidget::removeFeed, this, [this](int i){
            feedBottomWidget->articleModelChanged(nullptr);
            emit removeFeed(i);
        });
    }

private:
    Feed *currentFeed;
    FeedTopWidget *feedTopWidget;
    FeedBottomWidget *feedBottomWidget;


public slots:
    void FeedIndexChanged(const QModelIndex &index)
    {
        if (!index.isValid())
            return;

        Feed *selectedFeed = index.data(Qt::UserRole + 6).value<Feed *>();
        if (selectedFeed)
        {
            currentFeed = selectedFeed;
            feedTopWidget->setFeedDetails(
                QPixmap::fromImage(selectedFeed->image),
                selectedFeed->title,
                selectedFeed->link,
                selectedFeed->lastBuildDate,
                selectedFeed->url,
                selectedFeed->description,
                index.row());

            feedBottomWidget->articleModelChanged(selectedFeed->articles);
        }
    }
signals:
    void removeFeed(int);
};

#endif // FEEDWIDGET_HPP