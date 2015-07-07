#ifndef TWITTERMEDIA_H
#define TWITTERMEDIA_H

#include <QJsonObject>
#include <QJsonArray>
#include <QList>
#include "twitterentity.h"

struct TwiiterMediaSize
{
    int w;
    int h;
    QString resize;
};

class TwitterMedia : public TwitterEntity
{
public:
    TwitterMedia(const QJsonObject &jsonObj);
    TwitterMedia(const TwitterMedia &other);
    ~TwitterMedia();

    qint64 id();
    QString idStr();
    QString mediaUrl();
    QString mediaUrlHttps();
    QString url();
    QString displayUrl();
    QString expandedUrl();
    QList<TwiiterMediaSize> sizes();
    QString type();

private:
    QList<TwiiterMediaSize> sizes_;

};

#endif // TWITTERMEDIA_H
