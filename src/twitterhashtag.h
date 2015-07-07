#ifndef TWITTERHASHTAG_H
#define TWITTERHASHTAG_H

#include <QJsonObject>
#include <QList>
#include <QJsonArray>
#include "twitterentity.h"

class TwitterHashTag : public TwitterEntity
{
public:
    TwitterHashTag(const QJsonObject &jsonObj);
    TwitterHashTag(const TwitterHashTag &other);
    ~TwitterHashTag();

    QString text();

};

#endif // TWITTERHASHTAG_H
