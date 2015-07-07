#include "tweet.h"

Tweet::Tweet()
{

}

Tweet::Tweet(const QJsonObject &jsonObj)
{
    obj_ = jsonObj;
    entities_ = TwitterEntities(obj_.value("entities").toObject());
    text_ = obj_.value("text").toString();
    //Scape text for proper entities inclusion
    QList<TwitterEntity> entities;
    foreach (TwitterHashTag hashtag, entities_.hashtags()) {
        entities.push_back(hashtag);
    }
    foreach (TwitterMedia media, entities_.media()) {
        entities.push_back(media);
    }
    foreach (TwitterSymbol symbol, entities_.symbols()) {
        entities.push_back(symbol);
    }
    foreach (TwitterUrl url, entities_.urls()) {
        entities.push_back(url);
    }
    foreach (TwitterUserMention userMention, entities_.userMentions()) {
        entities.push_back(userMention);
    }

}

Tweet::Tweet(const Tweet &other)
{
    obj_        = other.obj_;
    entities_   = other.entities_;
    text_       = other.text_;
    scapedText_ = other.scapedText_;
}

Tweet::~Tweet()
{

}

QString Tweet::coordinates()
{
    return obj_.value("coordinates").toString();
}

QString Tweet::createdAt()
{
    return obj_.value("created_at").toString();
}

TwitterEntities &Tweet::entities()
{
    return entities_;
}

int Tweet::favoriteCount()
{
    return obj_.value("favorite_count").toInt();
}

bool Tweet::favorited()
{
    return obj_.value("favorited").toBool();
}

QString Tweet::filterLevel()
{
    return obj_.value("filter_level").toString();
}

qint64 Tweet::id()
{
    return idStr().toLongLong();
}

QString Tweet::idStr()
{
    return obj_.value("id_str").toString();
}

QString Tweet::inReplyToScreenName()
{
    return obj_.value("in_reply_to_screen_name").toString();
}

qint64 Tweet::inReplyToStatusId()
{
    return inReplyToStatusIdStr().toLongLong();
}

QString Tweet::inReplyToStatusIdStr()
{
    return obj_.value("in_reply_to_status_id_str").toString();
}

qint64 Tweet::inReplyToUserId()
{
    return inReplyToUserIdStr().toLongLong();
}

QString Tweet::inReplyToUserIdStr()
{
    return obj_.value("in_reply_to_user_id_str").toString();
}

QString Tweet::lang()
{
    return obj_.value("lang").toString();
}

bool Tweet::possiblySensitive()
{
    return obj_.value("possibly_sensitive").toBool();
}

int Tweet::retweetCount()
{
    return obj_.value("retweet_count").toInt();
}

bool Tweet::retweeted()
{
    return obj_.value("retweeted").toBool();
}

Tweet Tweet::retweetedStatus()
{
    return Tweet(obj_.value("retweeted_status").toObject());
}

QString Tweet::source()
{
    return obj_.value("source").toString();
}

QString Tweet::text()
{
    return text_;
}

bool Tweet::truncated()
{
    return obj_.value("truncated").toBool();
}

TwitterUser Tweet::user()
{
    return TwitterUser(obj_.value("user").toObject());
}

bool Tweet::withheldCopyright()
{
    return obj_.value("withheld_copyright").toBool();
}

const QJsonObject &Tweet::jsonObj()
{
    return obj_;
}
