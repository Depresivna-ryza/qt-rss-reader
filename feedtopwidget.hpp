#ifndef FEEDTOPWIDGET_HPP
#define FEEDTOPWIDGET_HPP

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QModelIndex>
#include <QScrollArea>
#include <QPushButton>

#include "feed.hpp"
#include "feeddetailspopup.hpp"

/*
* @brief FeedTopWidget class
* This class is the top part of the FeedWidget.
* It contains the details of the selected feed.
*/
class FeedTopWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FeedTopWidget(QWidget *parent = nullptr)
    {
        setAttribute(Qt::WA_StyledBackground);

        imageLabel = new QLabel(this);
        imageLabel->setFixedSize(200, 120);

        titleLabel = new QLabel(this);
        linkLabel = new QLabel(this);
        lastBuildDateLabel = new QLabel(this);
        urlLabel = new QLabel(this);
        descriptionLabel = new QLabel(this);

        imageLabel->setObjectName("rightLabel");
        titleLabel->setObjectName("rightLabelTitle");
        linkLabel->setObjectName("rightLabelSmall");
        lastBuildDateLabel->setObjectName("rightLabelSmall");
        urlLabel->setObjectName("rightLabelSmall");
        descriptionLabel->setObjectName("rightLabel");

        titleLabel->setWordWrap(true);
        linkLabel->setWordWrap(true);
        lastBuildDateLabel->setWordWrap(true);
        urlLabel->setWordWrap(true);
        descriptionLabel->setWordWrap(true);


        QHBoxLayout *topLayout = new QHBoxLayout;
        topLayout->addWidget(imageLabel);

        QVBoxLayout *textLayout = new QVBoxLayout;

        textLayout->setContentsMargins(0, 0, 0, 0);
        textLayout->setSpacing(0);
        textLayout->addWidget(titleLabel);
        textLayout->addWidget(lastBuildDateLabel);
        textLayout->addWidget(linkLabel);
        textLayout->addWidget(urlLabel);
        textLayout->addWidget(descriptionLabel);

        QScrollArea *scrollArea = new QScrollArea(this);
        scrollArea->setObjectName("rightScrollArea");
        scrollArea->setWidgetResizable(true);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

        QWidget *textWidget = new QWidget;
        textWidget->setLayout(textLayout);
        // textWidget->setMinimumWidth(500);
        scrollArea->setWidget(textWidget);

        removeButton = new QPushButton(tr("Unsubscribe from feed"), this);
        removeButton->setIcon(QIcon(":/resources/delete.png"));
        removeButton->setCursor(Qt::PointingHandCursor);
        removeButton->setFlat(true);
        removeButton->setObjectName("removeButton");
        removeButton->setFixedSize(200, 40);
        removeButton->hide();
        connect(removeButton, &QPushButton::clicked, this, [this]()
                { emit removeFeed(feedIndex);
                clearFeedDetails(); });

        QHBoxLayout *headerLayout = new QHBoxLayout;
        headerLayout->addStretch(1);
        headerLayout->addWidget(removeButton);

        QVBoxLayout *rightColumnLayout = new QVBoxLayout;
        rightColumnLayout->addLayout(headerLayout);

        rightColumnLayout->addWidget(scrollArea);

        topLayout->addLayout(rightColumnLayout);

        setLayout(topLayout);
    }
    void setFeedDetails(const QPixmap &image, QString title, QString link,
                        QString lastBuildDate, QString url, QString description, int index)
    {
        QPixmap scaledImage;
        if (image.isNull())
        {
            QPixmap noimage = QPixmap(":/resources/noimage.png");
            scaledImage = noimage.scaled(maxImageSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        }
        else
        {
            scaledImage = image.scaled(maxImageSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        }

        if (title.isEmpty()) title = "<no_title>";
        if (link.isEmpty()) link = "<empty>";
        if (lastBuildDate.isEmpty()) lastBuildDate = "<empty>";
        if (url.isEmpty()) url = "<empty>";


        imageLabel->setPixmap(scaledImage);
        titleLabel->setText(title);
        linkLabel->setText("link:         " + link);
        lastBuildDateLabel->setText("fetched:  " + lastBuildDate);
        urlLabel->setText("rss:          " + url);
        descriptionLabel->setText(description);

        if (title.isEmpty())
        {
            removeButton->hide();
        }
        else
        {
            removeButton->show();
        }

        feedIndex = index;
    }

    void clearFeedDetails()
    {
        imageLabel->clear();
        titleLabel->clear();
        linkLabel->clear();
        lastBuildDateLabel->clear();
        urlLabel->clear();
        descriptionLabel->clear();
        removeButton->hide();
    }

private:
    QLabel *imageLabel;
    QLabel *titleLabel;
    QLabel *linkLabel;
    QLabel *urlLabel;
    QLabel *lastBuildDateLabel;
    QLabel *descriptionLabel;

    QPushButton *removeButton;
    int feedIndex = -1;

    const QSize maxImageSize = QSize(200, 120);

signals:
    void removeFeed(int i);
};

#endif // FEEDTOPWIDGET_HPP