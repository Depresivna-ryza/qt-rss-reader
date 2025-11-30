#ifndef FEEDBOTTOMWIDGET_HPP
#define FEEDBOTTOMWIDGET_HPP

#include <QWidget>
#include <QListView>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QModelIndex>
#include <QScrollArea>
#include <QTextBrowser>

#include "feed.hpp"
#include "article.hpp"
/*
* @brief FeedBottomWidget class
* This class is the bottom part of the FeedWidget.
* It contains a list of all the articles of the selected feed and
* the selected article.
*/
class FeedBottomWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FeedBottomWidget(QWidget *parent = nullptr)
    {
        setAttribute(Qt::WA_StyledBackground);

        articleListView = new QListView(this);
        articleListView->setObjectName("articleListView");
        articleListView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        articleListView->setFixedWidth(200);
        // articleListView->setContentsMargins(0, 0, 0, 0);

        QLabel * articlesString = new QLabel(tr("Articles"));
        articlesString->setObjectName("articlesString");
        articlesString->setFixedWidth(200);
        // articlesString->setAlignment(Qt::AlignHCenter);

        QVBoxLayout *articlesLayout = new QVBoxLayout(this);
        articlesLayout->setSpacing(0);
        articlesLayout->addWidget(articlesString);
        articlesLayout->addWidget(articleListView);
        // articlesLayout->setContentsMargins(0, 0, 0, 0);

        QWidget *articlesWidget = new QWidget(this);
        articlesWidget->setFixedWidth(220);
        articlesWidget->setObjectName("articlesWidget");
        articlesWidget->setLayout(articlesLayout);

        titleLabel = new QLabel(this);
        linkLabel = new QLabel(this);
        pubDateLabel = new QLabel(this);
        authorLabel = new QLabel(this);
        enclosureLabel = new QLabel(this);
        descriptionLabel = new QLabel(this);

        titleLabel->setWordWrap(true);
        linkLabel->setWordWrap(true);
        pubDateLabel->setWordWrap(true);
        authorLabel->setWordWrap(true);
        enclosureLabel->setWordWrap(true);
        descriptionLabel->setWordWrap(true);

        descriptionLabel->setTextInteractionFlags(Qt::NoTextInteraction);
        descriptionLabel->setTextFormat(Qt::RichText);

        titleLabel->setObjectName("rightLabelTitleArticle");
        linkLabel->setObjectName("rightLabelSmallArticle");
        pubDateLabel->setObjectName("rightLabelSmallArticle");
        authorLabel->setObjectName("rightLabelSmallArticle");
        enclosureLabel->setObjectName("rightLabelSmallArticle");
        descriptionLabel->setObjectName("rightLabelArticle");

        rightColumnLayout = new QVBoxLayout();
        rightColumnLayout->setSizeConstraint(QLayout::SetMinimumSize);

        rightColumnLayout->addWidget(titleLabel);
        rightColumnLayout->addWidget(linkLabel);
        rightColumnLayout->addWidget(pubDateLabel);
        rightColumnLayout->addWidget(authorLabel);
        rightColumnLayout->addWidget(enclosureLabel);
        rightColumnLayout->addWidget(descriptionLabel);

        rightColumnLayout->setSpacing(0);

        QScrollArea *scrollArea = new QScrollArea(this);
        scrollArea->setObjectName("articleScrollArea");
        scrollArea->setWidgetResizable(true);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

        QWidget *scrollAreaContent = new QWidget;
        scrollAreaContent->setLayout(rightColumnLayout);
        scrollArea->setWidget(scrollAreaContent);

        QHBoxLayout *mainLayout = new QHBoxLayout(this);
        mainLayout->addWidget(articlesWidget, 1); 
        mainLayout->addWidget(scrollArea, 2);      

        connect(articleListView, &QListView::clicked,
                this, &FeedBottomWidget::onArticleSelected);

        // mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setSpacing(15);

        setLayout(mainLayout);
    }

    void articleModelChanged(ArticleModel *articleModel)
    {
        articleListView->setModel(articleModel);
        this->articleModel = articleModel;
        clearSelectedArticle();
    }

    void clearSelectedArticle()
    {
        titleLabel->setText("");
        linkLabel->setText("");
        pubDateLabel->setText("");
        authorLabel->setText("");
        enclosureLabel->setText("");
        descriptionLabel->setText("");
    }

public slots:

    void onArticleSelected(const QModelIndex &index)
    {
        if (!index.isValid())
            return;

        Article selectedArticle = index.data(Qt::UserRole + 5).value<Article>();
        titleLabel->setText(selectedArticle.title);
        linkLabel->setText(selectedArticle.link);
        pubDateLabel->setText(selectedArticle.pubDate);
        authorLabel->setText(selectedArticle.author);
        enclosureLabel->setText(selectedArticle.enclosure);
        descriptionLabel->setText(selectedArticle.description);

        qDebug() << selectedArticle.description;
    }

private:
    ArticleModel *articleModel;

    QListView *articleListView;
    QLabel *titleLabel;
    QLabel *linkLabel;
    QLabel *pubDateLabel;
    QLabel *authorLabel;
    QLabel *enclosureLabel;
    QLabel *descriptionLabel;
    QVBoxLayout *rightColumnLayout;
};

#endif // FEEDBOTTOMWIDGET_HPP