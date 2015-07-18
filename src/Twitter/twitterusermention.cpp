#include "twitterusermention.h"

TwitterUserMention::TwitterUserMention(const QJsonObject &jsonObj) :
    TwitterEntity(jsonObj)
{

}

TwitterUserMention::TwitterUserMention(const TwitterUserMention &other) :
    TwitterEntity(other)
{

}

TwitterUserMention::~TwitterUserMention()
{

}

qint64 TwitterUserMention::id()
{
    return idStr().toLongLong();
}

QString TwitterUserMention::idStr()
{
    return obj_.value("id_str").toString();
}

QString TwitterUserMention::screenName()
{
    return obj_.value("screen_name").toString();
}

QString TwitterUserMention::name()
{
    return obj_.value("name").toString();
}
