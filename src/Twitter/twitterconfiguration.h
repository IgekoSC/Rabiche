#ifndef TWITTERCONFIGURATION_H
#define TWITTERCONFIGURATION_H

#include <QJsonObject>
#include <QMap>
#include <QVariantList>
#include <QJsonArray>
#include "twittermedia.h"

class TwitterConfiguration
{
public:
    TwitterConfiguration();
    TwitterConfiguration(const QJsonObject &jsonObj);
    TwitterConfiguration(const TwitterConfiguration &other);
    int dmTextCharacterLimit();
    int charactersReservedPerMedia();
    int maxMediaPerUpload();
    QVariantList nonUsernamePaths();
    int photoSizeLimit();
    int shortUrlLength();
    int shortUrlLengthHttps();
    TwiiterMediaSize sizeThumb() const;
    TwiiterMediaSize sizeLarge() const;
    TwiiterMediaSize sizeMedium() const;
    TwiiterMediaSize sizeSmall() const;

private:
    QJsonObject obj_;
    TwiiterMediaSize sizeThumb_;
    TwiiterMediaSize sizeLarge_;
    TwiiterMediaSize sizeMedium_;
    TwiiterMediaSize sizeSmall_;



};

#endif // TWITTERCONFIGURATION_H
