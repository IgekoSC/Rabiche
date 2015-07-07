#include "twitterhashtag.h"

TwitterHashTag::TwitterHashTag(const QJsonObject &jsonObj) :
    TwitterEntity(jsonObj)
{

}

TwitterHashTag::TwitterHashTag(const TwitterHashTag &other) :
    TwitterEntity(other)
{

}

TwitterHashTag::~TwitterHashTag()
{

}

QString TwitterHashTag::text()
{
    return obj_.value("text").toString();
}
