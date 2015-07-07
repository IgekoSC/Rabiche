#ifndef TWITTER_ENTITIES_H
#define TWITTER_ENTITIES_H

#include "twittermedia.h"
#include "twitterurl.h"
#include "twitterusermention.h"
#include "twitterhashtag.h"
#include "twittersymbol.h"

class TwitterEntities
{
public:
    TwitterEntities();
    TwitterEntities(const QJsonObject &jsonObj);
    TwitterEntities(const TwitterEntities &other);
    ~TwitterEntities();
    QList<TwitterMedia> &media();
    QList<TwitterUrl> &urls();
    QList<TwitterUserMention> &userMentions();
    QList<TwitterHashTag> &hashtags();
    QList<TwitterSymbol> &symbols();

private:
    QJsonObject obj_;
    QList<TwitterMedia> media_;
    QList<TwitterUrl> urls_;
    QList<TwitterUserMention> userMentions_;
    QList<TwitterHashTag> hashtags_;
    QList<TwitterSymbol> symbols_;

};

#endif // TWITTER_ENTITIES_H
