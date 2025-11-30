#ifndef FEEDDETAILSPOPUP_HPP
#define FEEDDETAILSPOPUP_HPP

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QHBoxLayout>

class FeedDetailsPopup : public QWidget
{
    Q_OBJECT

public:
    explicit FeedDetailsPopup(QWidget *parent = nullptr) : QWidget(parent)
    {
        QVBoxLayout *layout = new QVBoxLayout(this);

        QLabel *titleTitleLabel = new QLabel(tr("Title:"), this);
        titleLabel = new QLabel(this);
        titleLabel->setWordWrap(true);
        layout->addWidget(titleTitleLabel);
        layout->addWidget(titleLabel);

        QLabel *linkTitleLabel = new QLabel(tr("Link:"), this);
        linkLabel = new QLabel(this);
        linkLabel->setWordWrap(true);
        layout->addWidget(linkTitleLabel);
        layout->addWidget(linkLabel);

        QLabel *lastBuildDateTitleLabel = new QLabel(tr("Last Build Date:"), this);
        lastBuildDateLabel = new QLabel(this);
        lastBuildDateLabel->setWordWrap(true);
        layout->addWidget(lastBuildDateTitleLabel);

        QLabel *urlTitleLabel = new QLabel(tr("URL:"), this);
        urlLabel = new QLabel(this);
        urlLabel->setWordWrap(true);
        layout->addWidget(urlTitleLabel);

        QPushButton *removeButton = new QPushButton(tr("Remove Feed"), this);
        connect(removeButton, &QPushButton::clicked, this, &FeedDetailsPopup::removeFeedRequested);
        layout->addWidget(removeButton);
    }

    void setFeedDetails(const QString &title, const QString &link, const QString &lastBuildDate, const QString &url)
    {
        titleLabel->setText(title);
        linkLabel->setText(link);
        lastBuildDateLabel->setText(lastBuildDate);
        urlLabel->setText(url);
    }

signals:
    void removeFeedRequested();

private:
    QLabel *titleLabel;
    QLabel *linkLabel;
    QLabel *lastBuildDateLabel;
    QLabel *urlLabel;
    QLabel *enclosureLabel;
};

#endif // FEEDDETAILSPOPUP_HPP