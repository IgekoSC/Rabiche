#ifndef TWITTERHTMLBRIDGE_H
#define TWITTERHTMLBRIDGE_H

#include <QWebView>
#include <QWebFrame>
#include <QThread>
#include <Twitter/twitter.h>

class TwitterHtmlBridge : public QObject
{
    Q_OBJECT
public:
    TwitterHtmlBridge(Twitter* twitter, QWebPage* webPage, QObject *parent = 0);
    ~TwitterHtmlBridge();
    Q_INVOKABLE QMap<QString, QVariant> newHtmlTweets();
    Q_INVOKABLE QMap<QString, QVariant> newOfflineHtmlTweets();
    Q_INVOKABLE void tweet(QString text);

private slots:
    void onNewTweets(TweetsMap tweets);
    void onNewOfflineTweets(TweetsMap tweets);
    void onLinkClicked(const QUrl &url);
    void loadFinished(bool b);
    void onJavaScriptWindowObjectCleared();

private:
    bool firstTime_;
    Twitter* twitter_;
    QWebPage* webPage_;
    QMap<QString, QVariant> htmlTweets_;
    QMap<QString, QVariant> newHtmlTweets_;
    QMap<QString, QVariant> newOfflineHtmlTweets_;
    int page_;
    int pageSize_;

    void init();
    QMap<QString, QVariant> twittsToHtml(TweetsMap twitts);
    QString htmlPagination();
    void goToPage(int page);
};

#endif // TWITTERHTMLBRIDGE_H
