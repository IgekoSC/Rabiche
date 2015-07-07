#include "twitteruser.h"

TwitterUser::TwitterUser(const QJsonObject &jsonObj)
{
    obj_ = jsonObj;
    entities_ = TwitterEntities(obj_.value("entities").toObject());
}

TwitterUser::TwitterUser(const TwitterUser &other)
{
    obj_ = other.obj_;
    entities_ = other.entities_;
}

TwitterUser::~TwitterUser()
{

}

bool TwitterUser::contributorsEnabled()
{
    return obj_.value("contributors_enabled").toBool();
}

QString TwitterUser::createdAt()
{
    return obj_.value("created_at").toString();
}

bool TwitterUser::defaultProfile()
{
    return obj_.value("default_profile").toBool();
}

QString TwitterUser::description()
{
    return obj_.value("description").toString();
}

TwitterEntities TwitterUser::entities()
{
    return entities_;
}

int TwitterUser::favouritesCount()
{
    return obj_.value("favourites_count").toInt();
}

bool TwitterUser::followRequestSent()
{
    return obj_.value("follow_request_sent").toBool();
}

bool TwitterUser::following()
{
    return obj_.value("following").toBool();
}

int TwitterUser::followersCount()
{
    return obj_.value("followers_count").toInt();
}

int TwitterUser::friendsCount()
{
    return obj_.value("friends_count").toInt();
}

bool TwitterUser::geoEnabled()
{
    return obj_.value("geo_enabled").toBool();
}

qint64 TwitterUser::id()
{
    return idStr().toLongLong();
}

QString TwitterUser::idStr()
{
    return obj_.value("id_str").toString();
}

bool TwitterUser::isTranslator()
{
    return obj_.value("is_translator").toBool();
}

QString TwitterUser::lang()
{
    return obj_.value("lang").toString();
}

int TwitterUser::listedCount()
{
    return obj_.value("listed_count").toInt();
}

QString TwitterUser::location()
{
    return obj_.value("location").toString();
}

QString TwitterUser::name()
{
    return obj_.value("name").toString();
}

bool TwitterUser::notifications()
{
    return obj_.value("notifications").toBool();
}

QString TwitterUser::profileBackgroundColor()
{
    return obj_.value("profile_background_color").toString();
}

QString TwitterUser::profileBackgroundImageUrl()
{
    return obj_.value("profile_background_image_url").toString();
}

QString TwitterUser::profileBackgroundImageUrlHttps()
{
    return obj_.value("profile_background_image_url_https").toString();
}

bool TwitterUser::profileBackgroundTile()
{
    return obj_.value("profile_background_tile").toBool();
}

QString TwitterUser::profileBannerUrl()
{
    return obj_.value("profile_banner_url").toString();
}

QString TwitterUser::profileImageUrl()
{
    return obj_.value("profile_image_url").toString();
}

QString TwitterUser::profileImageUrlHttps()
{
    return obj_.value("profile_image_url_https").toString();
}

QString TwitterUser::profileLinkColor()
{
    return obj_.value("profile_link_color").toString();
}

QString TwitterUser::profileSidebarBorderColor()
{
    return obj_.value("profile_sidebar_border_color").toString();
}

QString TwitterUser::profileTextColor()
{
    return obj_.value("profile_text_color").toString();
}

bool TwitterUser::profileUseBackgroundImage()
{
    return obj_.value("profile_use_background_image").toBool();
}

bool TwitterUser::isProtected()
{
    return obj_.value("protected").toBool();
}

QString TwitterUser::screenName()
{
    return obj_.value("screen_name").toString();
}

bool TwitterUser::showAllInlineMedia()
{
    return obj_.value("show_all_inline_media").toBool();
}

Tweet TwitterUser::status()
{
    return Tweet(obj_.value("key").toObject());
}

int TwitterUser::statusesCount()
{
    return obj_.value("statuses_count").toInt();
}

QString TwitterUser::timeZone()
{
    return obj_.value("time_zone").toString();
}

QString TwitterUser::url()
{
    return obj_.value("url").toString();
}

int TwitterUser::utcOffset()
{
    return obj_.value("utc_offset").toInt();
}

bool TwitterUser::verified()
{
    return obj_.value("verified").toBool();
}
