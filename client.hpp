#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QListView>
#include <QTextBrowser>
#include <QSplitter>
#include <QVBoxLayout>
#include <QPushButton>
#include <QInputDialog>
#include <QLabel>
#include <QFile>
#include <QApplication>

#include "article.hpp"
#include "rss.hpp"
#include "feed.hpp"
#include "feedmanager.hpp"
#include "feedlistwidget.hpp"
#include "feedwidget.hpp"

/*
 * @brief Client class
 * This class is the main window of the application.
 * It contains a FeedListWidget and a FeedWidget.
 * The FeedListWidget is a list of all the feeds.
 * The FeedWidget is a widget that displays the articles of the selected feed.
 */
class Client : public QWidget
{
    Q_OBJECT

public:
    Client(QWidget *parent = nullptr)
    {
        feedManager = new FeedManager(this);
        setFocusPolicy(Qt::StrongFocus);
        resize(1600, 900);
        setWindowTitle(tr("RSS Client"));
        loadStyleSheet();
        setupUI();

        connect(feedWidget, &FeedWidget::removeFeed, feedManager, &FeedManager::removeFeed);
    }
    virtual ~Client() = default;

private:
    FeedManager *feedManager;
    FeedWidget *feedWidget;
    FeedListWidget *feedListWidget;

    void loadStyleSheet()
    {
        QFile file(":/resources/style.qss");
        file.open(QFile::ReadOnly);
        QTextStream stream(&file);
        QString style = stream.readAll();
        qDebug() << "Style Sheet " << style;
        qApp->setStyleSheet(style);
        file.close();
    }

    void setupUI()
    {

        feedWidget = new FeedWidget(this);

        feedListWidget = new FeedListWidget(feedManager, this);
        
        connect(feedListWidget->getFeedListView(), &QListView::clicked, feedWidget, &FeedWidget::FeedIndexChanged);

        QHBoxLayout *mainLayout = new QHBoxLayout(this);
        mainLayout->addWidget(feedListWidget);
        mainLayout->addWidget(feedWidget, 1);

        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setSpacing(10);

        setLayout(mainLayout);
    }
};

#endif // CLIENT_H
