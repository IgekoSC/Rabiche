#include "twitterentities.h"
#include <QDebug>

TwitterEntities::TwitterEntities()
{

}

TwitterEntities::TwitterEntities(const QJsonObject &jsonObj)
{
    obj_ = jsonObj.value("entities").toObject();

    QJsonArray array = jsonObj.value("extended_entities").toObject().value("media").toArray();
    foreach (QJsonValue value, array) {
        media_.push_back(TwitterMedia(value.toObject()));
    }

    array = obj_.value("urls").toArray();
    foreach (QJsonValue value, array) {
        urls_.push_back(TwitterUrl(value.toObject()));
    }

    array = obj_.value("user_mentions").toArray();
    foreach (QJsonValue value, array) {
        userMentions_.push_back(TwitterUserMention(value.toObject()));
    }

    array = obj_.value("hashtags").toArray();
    foreach (QJsonValue value, array) {
        hashtags_.push_back(TwitterHashTag(value.toObject()));
    }

    array = obj_.value("symbols").toArray();
    foreach (QJsonValue value, array) {
        symbols_.push_back(TwitterSymbol(value.toObject()));
    }
}

TwitterEntities::TwitterEntities(const TwitterEntities &other)
{
    obj_ = other.obj_;
    media_ = other.media_;
    urls_ = other.urls_;
    userMentions_ = other.userMentions_;
    hashtags_ = other.hashtags_;
    symbols_ = other.symbols_;
}

TwitterEntities::~TwitterEntities()
{

}

QList<TwitterMedia> &TwitterEntities::media()
{
    return media_;
}

QList<TwitterUrl> &TwitterEntities::urls()
{
    return urls_;
}

QList<TwitterUserMention> &TwitterEntities::userMentions()
{
    return userMentions_;
}

QList<TwitterHashTag> &TwitterEntities::hashtags()
{
    return hashtags_;
}

QList<TwitterSymbol> &TwitterEntities::symbols()
{
    return symbols_;
}
