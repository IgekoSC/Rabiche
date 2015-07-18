#ifndef TWEET_H
#define TWEET_H

#include <QJsonObject>
#include <QDateTime>
#include <QVariant>
#include "twitteruser.h"
#include "twitterentities.h"

class TwitterUser;

class Tweet
{
public:
    Tweet();
    Tweet(const QJsonObject &jsonObj);
    Tweet(const Tweet &other);
    ~Tweet();

    QString coordinates();
    QString createdAt();
    TwitterEntities &entities();
    int favoriteCount();
    bool favorited();
    QString filterLevel();
    qint64 id();
    QString idStr();
    QString inReplyToScreenName();
    qint64 inReplyToStatusId();
    QString inReplyToStatusIdStr();
    qint64 inReplyToUserId();
    QString inReplyToUserIdStr();
    QString lang();
    bool possiblySensitive();
    int retweetCount();
    bool retweeted();
    Tweet retweetedStatus();
    QString source();
    QString text();
    bool truncated();
    TwitterUser user();
    bool withheldCopyright();
    const QJsonObject &jsonObj();

private:
    QJsonObject obj_;
    TwitterEntities entities_;
    QString text_;
    QString htmlText_;
    bool lastOnClose_;

};

Q_DECLARE_METATYPE(Tweet)

typedef QMap<qint64, Tweet> TweetsMap;

#endif // TWEET_H
