#ifndef TWITTER_H
#define TWITTER_H

#include <QObject>
#include <QString>
#include <QUrl>
#include <QVariantMap>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>

#include <SqlSettings/sqlsettings.h>
#include <SynchronousHttpRequest/synchronoushttprequest.h>
#include <o1/o1twitter.h>
#include <o1/signedrequester.h>
#include <QMutexLocker>
#include "tweet.h"
#include "twitterstream.h"

#define TWITTER_USER_STREAM_URI "https://userstream.twitter.com/1.1/user.json"
#define TWITTER_REST_LOOKUP_URI "https://api.twitter.com/1.1/statuses/lookup.json"
#define TWITTER_HOME_TIMELINE   "https://api.twitter.com/1.1/statuses/home_timeline.json"

#define TWITTER_CUSTOMER_KEY    "qv9DUeDWumhUydA2TDnj7zPk3"
#define TWITTER_CUSTOMER_SECRET "UO5zz7oVRvrs9USUF7HU52usdqXpYH5Wp6Zs4MelSPTQYtNp8a"

class Twitter : public QObject
{
    Q_OBJECT
public:
    explicit Twitter(QString name, QObject *parent = 0);
    ~Twitter();

    const TweetsMap &tweets();
    unsigned char loginState();
    unsigned char streamConnectionState();

public slots:
    void login();
    void loguout();
    void connectToStream(QString uri);

signals:
    void newTweets(TweetsMap tweets);
    void newOfflineTweets(TweetsMap tweets);

private slots:
    void onStreamNewTweets();
    void onRefreshTimerTimeout();

private:
    QSqlDatabase db_;
    SqlSettings* settings_;
    QDir dataDir_;
    QDir cacheJsonDir_;
    QDir cacheMediaDir_;
    SynchronousHttpRequest* shr_;
    O1Twitter* auth_;
    SignedRequester* req_;
    qint64 newest_id_;
    qint64 oldest_id_;
    TweetsMap tweets_;
    TwitterStream* stream_;
    QMutex mutex_;
    unsigned char loginState_;
    unsigned char streamConnectionState_;
    QTimer* refreshTimer_;
    qint64 lastRefreshedTweetId_;

    QJsonArray homeTimeline(qint64 max_id = 0, qint64 since_id = 0, int count = 200);
    void updateTweetsCache(const TweetsMap &tweets);
    void updateIds();
    void updateTweetsData();
    void recoverOfflineTweets();
};

#endif // TWITTER_H
