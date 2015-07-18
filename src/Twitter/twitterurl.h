#ifndef TWITTERURL_H
#define TWITTERURL_H

#include <QJsonObject>
#include <QList>
#include <QJsonArray>
#include "twitterentity.h"

class TwitterUrl : public TwitterEntity
{
public:
    TwitterUrl(const QJsonObject &jsonObj);
    TwitterUrl(const TwitterUrl &other);
    ~TwitterUrl();

    QString url();
    QString displayUrl();
    QString expandedUrl();

};

#endif // TWITTERURL_H
