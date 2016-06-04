#ifndef WGTNEWTWEET_H
#define WGTNEWTWEET_H

#include <QObject>
#include <QWidget>
#include <QGridLayout>
#include <QToolButton>
#include <QTextEdit>
#include <QLabel>
#include <QListWidget>
#include <QMenu>
#include <QAction>
#include <Twitter/twitterconfiguration.h>
#include <Twitter/twitterstatus.h>

class WgtNewTweet : public QWidget
{
    Q_OBJECT
public:
    WgtNewTweet(const TwitterConfiguration& twitterConfiguration, QWidget* parent = 0);

signals:
    void updateStatus(TwitterStatus status, QStringList mediaPaths);

private slots:
    void onTextChanged();
    void onBtnTweetPressed();
    void onBtnAddMediaPressed();
    void onActDeleteMediaTriggered();

private:
    QGridLayout* mainLayout_;
    QTextEdit* txtTweet_;
    QToolButton* btnTweet_;
    QToolButton* btnAddMedia_;
    QLabel* lblChars_;
    QListWidget* lstMedias_;
    QAction* actDeleteMedia_;
    int leftChars_;
    TwitterConfiguration twitterConfiguration_;
    int shortUrlLength_;
    int shortUrlLengthHttps_;

};

#endif // WGTNEWTWEET_H
