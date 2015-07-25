#include "twittermedia.h"

TwitterMedia::TwitterMedia(const QJsonObject &jsonObj) :
    TwitterEntity(jsonObj)
{
    //sizes
    QJsonObject sizes = obj_.value("sizes").toObject();
    TwiiterMediaSize size;

    size.w = sizes.value("thumb").toObject().value("w").toInt();
    size.h = sizes.value("thumb").toObject().value("h").toInt();
    size.resize = sizes.value("thumb").toObject().value("resize").toString();
    sizeThumb_ = size;

    size.w = sizes.value("large").toObject().value("w").toInt();
    size.h = sizes.value("large").toObject().value("h").toInt();
    size.resize = sizes.value("large").toObject().value("resize").toString();
    sizeLarge_ = size;

    size.w = sizes.value("medium").toObject().value("w").toInt();
    size.h = sizes.value("medium").toObject().value("h").toInt();
    size.resize = sizes.value("medium").toObject().value("resize").toString();
    sizeMedium_ = size;

    size.w = sizes.value("small").toObject().value("w").toInt();
    size.h = sizes.value("small").toObject().value("h").toInt();
    size.resize = sizes.value("small").toObject().value("resize").toString();
    sizeSmall_ = size;
}

TwitterMedia::TwitterMedia(const TwitterMedia &other) :
    TwitterEntity(other)
{
    sizeThumb_ = other.sizeThumb_;
    sizeLarge_ = other.sizeLarge_;
    sizeMedium_ = other.sizeMedium_;
    sizeSmall_ = other.sizeSmall_;
}

TwitterMedia::~TwitterMedia()
{

}

qint64 TwitterMedia::id()
{
    return idStr().toLongLong();
}

QString TwitterMedia::idStr()
{
    return obj_.value("id_str").toString();
}

QString TwitterMedia::mediaUrl()
{
    return obj_.value("media_url").toString();
}

QString TwitterMedia::mediaUrlHttps()
{
    return obj_.value("media_url_https").toString();
}

QString TwitterMedia::url()
{
    return obj_.value("url").toString();
}

QString TwitterMedia::displayUrl()
{
    return obj_.value("display_url").toString();
}

QString TwitterMedia::expandedUrl()
{
    return obj_.value("expanded_url").toString();
}

QString TwitterMedia::type()
{
    return obj_.value("type").toString();
}

const TwiiterMediaSize &TwitterMedia::sizeThumb() const
{
    return sizeThumb_;
}

const TwiiterMediaSize &TwitterMedia::sizeLarge() const
{
    return sizeLarge_;
}

const TwiiterMediaSize &TwitterMedia::sizeMedium() const
{
    return sizeMedium_;
}

const TwiiterMediaSize &TwitterMedia::sizeSmall() const
{
    return sizeSmall_;
}




