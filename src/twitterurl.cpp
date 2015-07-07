#include "twitterurl.h"

TwitterUrl::TwitterUrl(const QJsonObject &jsonObj) :
    TwitterEntity(jsonObj)
{

}

TwitterUrl::TwitterUrl(const TwitterUrl &other) :
    TwitterEntity(other)
{

}

TwitterUrl::~TwitterUrl()
{

}

QString TwitterUrl::url()
{
    return obj_.value("url").toString();
}

QString TwitterUrl::displayUrl()
{
    return obj_.value("display_url").toString();
}

QString TwitterUrl::expandedUrl()
{
    return obj_.value("expanded_url").toString();
}
