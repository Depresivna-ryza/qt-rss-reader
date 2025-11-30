#ifndef FEEDLISTWIDGET_HPP
#define FEEDLISTWIDGET_HPP

#include <QWidget>
#include <QListView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QInputDialog>
#include <QSortFilterProxyModel>

#include "feedmanager.hpp"
/*
* @brief FeedListWidget class
* This class is the left part of the Client.
* It contains a list of all the feeds.
*/
class FeedListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FeedListWidget(FeedManager *fm, QWidget *parent = nullptr) : QWidget(parent), feedManager(fm)
    {
        setAttribute(Qt::WA_StyledBackground);

        QLabel *rssIcon = new QLabel(this);
        rssIcon->setPixmap(QPixmap(":/resources/rss.png").scaled(30, 30, Qt::KeepAspectRatio));
        rssIcon->setAlignment(Qt::AlignLeft);

        QLabel *feedsLabel = new QLabel(tr("Feeds"));
        feedsLabel->setObjectName("feedsLabel");

        feedsLabel->setAlignment(Qt::AlignLeft);

        QHBoxLayout *rssTitleLayout= new QHBoxLayout(this);
        rssTitleLayout->addWidget(rssIcon);
        rssTitleLayout->addWidget(feedsLabel);
        rssTitleLayout->setContentsMargins(0, 0, 0, 0);
        rssTitleLayout->setSpacing(0);

        rssTitleLayout->addStretch(1);


        QWidget* rssTitleWidget = new QWidget(this);
        rssTitleWidget->setLayout(rssTitleLayout);

        searchBar = new QLineEdit(this);
        searchBar->setFixedHeight(20);

        searchBar->setObjectName("searchBar");
        searchBar->setPlaceholderText(tr("Search feeds..."));
        connect(searchBar, &QLineEdit::textChanged, this, &FeedListWidget::filterFeeds);

        feedListView = new QListView(this);
        feedListView->setObjectName("feedListView");
        feedListView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        feedListView->setWordWrap(false);

        proxyModel = new QSortFilterProxyModel(this);
        proxyModel->setSourceModel(feedManager->getFeeds());
        proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

        feedListView->setModel(proxyModel);

        addFeedLineEdit = new QLineEdit(this);
        addFeedLineEdit->setPlaceholderText(tr("Add new feed..."));
        addFeedLineEdit->setFixedHeight(30);
        addFeedLineEdit->setObjectName("addFeedLineEdit");
        addFeedButton = new QPushButton(this);
        addFeedButton->setIcon(QIcon(":/resources/plus2.png"));
        addFeedButton->setCursor(Qt::PointingHandCursor);
        addFeedButton->setFixedSize(40, 40);
        addFeedButton->setFlat(true);

        connect(addFeedButton, &QPushButton::clicked, this, [this]()
                {
                feedManager->addFeed(addFeedLineEdit->text());
                addFeedLineEdit->clear(); });

        QHBoxLayout *addFeedLayout = new QHBoxLayout(this);
        addFeedLayout->setSpacing(0);
        addFeedLayout->setContentsMargins(0, 0, 0, 0);
        addFeedLayout->addWidget(addFeedLineEdit);
        addFeedLayout->addWidget(addFeedButton);
        QWidget* addFeedWidget = new QWidget(this);
        addFeedWidget->setLayout(addFeedLayout);

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(rssTitleWidget);
        layout->addWidget(searchBar);
        layout->addWidget(feedListView);
        layout->addWidget(addFeedWidget);

        setLayout(layout);
        setFixedWidth(200);
    }

    QListView *getFeedListView() const { return feedListView; }

    void filterFeeds(const QString &text)
    {
        proxyModel->setFilterWildcard(text);
    }

private:
    FeedManager *feedManager;
    QListView *feedListView;
    QLineEdit *searchBar;
    QSortFilterProxyModel *proxyModel;

    QLineEdit *addFeedLineEdit;
    QPushButton *addFeedButton;
};

#endif // FEEDLISTWIDGET_HPP