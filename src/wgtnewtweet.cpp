#include "wgtnewtweet.h"
#include <QDebug>
#include <QStandardPaths>
#include <QFileDialog>

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
    lstMedias_ = new QListWidget(this);
    btnAddMedia_ = new QToolButton(this);

    actDeleteMedia_ = new QAction(tr("Delete"), lstMedias_);

    actDeleteMedia_->setEnabled(false);

    btnTweet_->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnTweet_->setText(tr("Tweet"));
    btnTweet_->setIcon(QIcon("://Resources/img/tweet.png"));
    btnTweet_->setIconSize(QSize(32, 32));
    btnTweet_->setEnabled(false);

    btnAddMedia_->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnAddMedia_->setText(tr("Add photo"));
    btnAddMedia_->setIcon(QIcon("://Resources/img/photo.png"));
    btnAddMedia_->setIconSize(QSize(32, 32));

    lstMedias_->setMaximumHeight(200);
    lstMedias_->setIconSize(QSize(96, 96));
    lstMedias_->setContextMenuPolicy(Qt::ActionsContextMenu);
    lstMedias_->addAction(actDeleteMedia_);

    mainLayout_->addWidget(txtTweet_,    0, 0, 1, 3);
    mainLayout_->addWidget(lstMedias_,   1, 0, 1, 3);
    mainLayout_->addWidget(lblChars_,    2, 0, 1, 1);
    mainLayout_->addWidget(btnAddMedia_, 2, 1, 1, 1);
    mainLayout_->addWidget(btnTweet_,    2, 2, 1, 1);

    connect(txtTweet_->document(), SIGNAL(contentsChanged()), this, SLOT(onTextChanged()));
    connect(btnTweet_, SIGNAL(clicked(bool)), this, SLOT(onBtnTweetPressed()));
    connect(btnAddMedia_, SIGNAL(clicked(bool)), this, SLOT(onBtnAddMediaPressed()));
    connect(actDeleteMedia_, SIGNAL(triggered(bool)), this, SLOT(onActDeleteMediaTriggered()));
}

void WgtNewTweet::onTextChanged()
{
    leftChars_ = 140 - txtTweet_->document()->toPlainText().size()
            - lstMedias_->count() * twitterConfiguration_.charactersReservedPerMedia();

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

    btnTweet_->setEnabled((leftChars_ >= -560) && (leftChars_ < 140));
}

void WgtNewTweet::onBtnTweetPressed()
{
    TwitterStatus status;

    status.setStatus(txtTweet_->toPlainText());
    txtTweet_->clear();

    QStringList mediaPaths;
    for (int i = 0; i < lstMedias_->count(); ++i) {
        mediaPaths.append(lstMedias_->item(i)->data(Qt::UserRole).toString());
    }

    emit updateStatus(status, mediaPaths);
}

void WgtNewTweet::onBtnAddMediaPressed()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Image"),
                                                    QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                                    tr("Image Files (*.png *.jpg *jpeg *.webp *.gif)"));

    if (fileName.isNull())
        return;

    QListWidgetItem* item = new QListWidgetItem(QIcon(fileName), fileName.section('/', -1));
    item->setData(Qt::UserRole, fileName);

    lstMedias_->addItem(item);

    btnAddMedia_->setEnabled(lstMedias_->count() < 4);
    actDeleteMedia_->setEnabled(lstMedias_->count() > 0);

    onTextChanged();
}

void WgtNewTweet::onActDeleteMediaTriggered()
{
    QListWidgetItem* item = lstMedias_->takeItem(lstMedias_->currentRow());

    delete item;

    btnAddMedia_->setEnabled(lstMedias_->count() < 4);
    actDeleteMedia_->setEnabled(lstMedias_->count() > 0);

    onTextChanged();
}
