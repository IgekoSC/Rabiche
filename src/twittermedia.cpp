#include "twittermedia.h"

TwitterMedia::TwitterMedia(const QJsonObject &jsonObj) :
    TwitterEntity(jsonObj)
{
    //sizes
    QJsonArray sizes = obj_.value("sizes").toArray();
    foreach (QJsonValue jsonSize, sizes) {
        TwiiterMediaSize size;
        size.w = jsonSize.toObject().value("w").toInt();
        size.h = jsonSize.toObject().value("h").toInt();
        size.resize = jsonSize.toObject().value("resize").toString();
        sizes_.push_back(size);
    }
}

TwitterMedia::TwitterMedia(const TwitterMedia &other) :
    TwitterEntity(other)
{
    sizes_ = other.sizes_;
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

QList<TwiiterMediaSize> TwitterMedia::sizes()
{
    return sizes_;
}

QString TwitterMedia::type()
{
    return obj_.value("type").toString();
}
