#ifndef TWITTERUSERMENTION_H
#define TWITTERUSERMENTION_H

#include <QJsonObject>
#include <QList>
#include <QJsonArray>
#include "twitterentity.h"

class TwitterUserMention : public TwitterEntity
{
public:
    TwitterUserMention(const QJsonObject &jsonObj);
    TwitterUserMention(const TwitterUserMention &other);
    ~TwitterUserMention();

    qint64 id();
    QString idStr();
    QString screenName();
    QString name();

};

#endif // TWITTERUSERMENTION_H
