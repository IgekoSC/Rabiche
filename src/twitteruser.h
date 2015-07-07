#ifndef TWITTERUSER_H
#define TWITTERUSER_H

#include <QJsonObject>
#include <QDateTime>
#include "tweet.h"
#include "twitterentities.h"

class Tweet;

class TwitterUser
{
public:
    TwitterUser(const QJsonObject &jsonObj);
    TwitterUser(const TwitterUser &other);
    ~TwitterUser();

    bool contributorsEnabled();
    QString createdAt();
    bool defaultProfile();
    bool defaultProfileImage();
    QString description();
    TwitterEntities entities();
    int favouritesCount();
    bool followRequestSent();
    bool following();
    int followersCount();
    int friendsCount();
    bool geoEnabled();
    qint64 id();
    QString idStr();
    bool isTranslator();
    QString lang();
    int listedCount();
    QString location();
    QString name();
    bool notifications();
    QString profileBackgroundColor();
    QString profileBackgroundImageUrl();
    QString profileBackgroundImageUrlHttps();
    bool profileBackgroundTile();
    QString profileBannerUrl();
    QString profileImageUrl();
    QString profileImageUrlHttps();
    QString profileLinkColor();
    QString profileSidebarBorderColor();
    QString profileSidebarFillColor();
    QString profileTextColor();
    bool profileUseBackgroundImage();
    bool isProtected();
    QString screenName();
    bool showAllInlineMedia();
    Tweet status();
    int statusesCount();
    QString timeZone();
    QString url();
    int utcOffset();
    bool verified();
    QString withheldInCountries();
    QString withheldScope();

private:
    QJsonObject obj_;
    TwitterEntities entities_;

};

#endif // TWITTERUSER_H
