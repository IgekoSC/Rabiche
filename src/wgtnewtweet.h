#ifndef WGTNEWTWEET_H
#define WGTNEWTWEET_H

#include <QObject>
#include <QWidget>
#include <QGridLayout>
#include <QToolButton>
#include <QTextEdit>
#include <QLabel>
#include <Twitter/twitterconfiguration.h>
#include <Twitter/twitterstatus.h>

class WgtNewTweet : public QWidget
{
    Q_OBJECT
public:
    WgtNewTweet(const TwitterConfiguration& twitterConfiguration, QWidget* parent = 0);

signals:
    void updateStatus(TwitterStatus status);

private slots:
    void onTextChanged();
    void onBtnTweetPressed();

private:
    QGridLayout* mainLayout_;
    QTextEdit* txtTweet_;
    QToolButton* btnTweet_;
    QLabel* lblChars_;
    int leftChars_;
    TwitterConfiguration twitterConfiguration_;
    int shortUrlLength_;
    int shortUrlLengthHttps_;

};

#endif // WGTNEWTWEET_H
