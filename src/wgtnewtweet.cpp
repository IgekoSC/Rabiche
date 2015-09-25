#include "wgtnewtweet.h"
#include <QDebug>

WgtNewTweet::WgtNewTweet(const TwitterConfiguration &twitterConfiguration, QWidget *parent) :
    QWidget(parent), twitterConfiguration_(twitterConfiguration)
{
    shortUrlLength_ = twitterConfiguration_.shortUrlLength();
    shortUrlLengthHttps_ = twitterConfiguration_.shortUrlLengthHttps();

    mainLayout_ = new QGridLayout(this);
    setLayout(mainLayout_);

    txtTweet_ = new QTextEdit(this);
    btnTweet_ = new QToolButton(this);
    lblChars_ = new QLabel("<span style=\"color:green;font-weight:bold;\">140</span>");

    btnTweet_->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnTweet_->setText(tr("Tweet"));
    btnTweet_->setIcon(QIcon("://Resources/img/tweet.png"));
    btnTweet_->setIconSize(QSize(32, 32));

    mainLayout_->addWidget(txtTweet_, 0, 0, 1, 2);
    mainLayout_->addWidget(lblChars_, 1, 0, 1, 1);
    mainLayout_->addWidget(btnTweet_, 1, 1, 1, 1);

    connect(txtTweet_->document(), SIGNAL(contentsChanged()), this, SLOT(onTextChanged()));
    connect(btnTweet_, SIGNAL(clicked(bool)), this, SLOT(onBtnTweetPressed()));
}

void WgtNewTweet::onTextChanged()
{
    leftChars_ = 140 - txtTweet_->document()->toPlainText().size();

    QRegExp reHttp("^https?:\\/\\/.*$");

    QStringList words = txtTweet_->document()->toPlainText().split(' ');
    int diff = 0;
    foreach (QString word, words) {
        if (reHttp.exactMatch(word)) {
            if (word.size() > shortUrlLength_) {
                int d = 0;
                if (word.at(4) == 's') {
                    d = word.size() - shortUrlLengthHttps_;
                } else {
                    d = word.size() - shortUrlLength_;
                }
                diff += d;
            }
        }
    }

    leftChars_ += diff;
    QString leftChars = QString::number(leftChars_);

    if (leftChars_ < -560) {
        lblChars_->setText("<span style=\"color:red;font-weight:bold;\">" + leftChars + "</span>");
    } else if (leftChars_ < 0) {
        lblChars_->setText("<span style=\"color:orange;font-weight:bold;\">" + leftChars + "</span>");
    } else {
        lblChars_->setText("<span style=\"color:green;font-weight:bold;\">" + leftChars + "</span>");
    }

    btnTweet_->setEnabled(leftChars_ >= 0);
}

void WgtNewTweet::onBtnTweetPressed()
{
    TwitterStatus status;

    status.setStatus(txtTweet_->toPlainText());
    txtTweet_->clear();

    emit updateStatus(status);
}
