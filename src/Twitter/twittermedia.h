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
    QString type();

    const TwiiterMediaSize &sizeThumb() const;
    const TwiiterMediaSize &sizeLarge() const;
    const TwiiterMediaSize &sizeMedium() const;
    const TwiiterMediaSize &sizeSmall() const;

private:
    TwiiterMediaSize sizeThumb_;
    TwiiterMediaSize sizeLarge_;
    TwiiterMediaSize sizeMedium_;
    TwiiterMediaSize sizeSmall_;

};

#endif // TWITTERMEDIA_H
