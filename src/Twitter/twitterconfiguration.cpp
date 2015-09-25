#include "twitterconfiguration.h"

TwitterConfiguration::TwitterConfiguration()
{

}

TwitterConfiguration::TwitterConfiguration(const QJsonObject &jsonObj)
{
    obj_ = jsonObj;

    QJsonObject sizes = obj_.value("photo_sizes").toObject();

    TwiiterMediaSize size;

    size.w = sizes.value("thumb").toObject().value("w").toInt();
    size.h = sizes.value("thumb").toObject().value("h").toInt();
    size.resize = sizes.value("thumb").toObject().value("resize").toString();
    sizeThumb_ = size;

    size.w = sizes.value("large").toObject().value("w").toInt();
    size.h = sizes.value("large").toObject().value("h").toInt();
    size.resize = sizes.value("large").toObject().value("resize").toString();
    sizeLarge_ = size;

    size.w = sizes.value("medium").toObject().value("w").toInt();
    size.h = sizes.value("medium").toObject().value("h").toInt();
    size.resize = sizes.value("medium").toObject().value("resize").toString();
    sizeMedium_ = size;

    size.w = sizes.value("small").toObject().value("w").toInt();
    size.h = sizes.value("small").toObject().value("h").toInt();
    size.resize = sizes.value("small").toObject().value("resize").toString();
    sizeSmall_ = size;
}

TwitterConfiguration::TwitterConfiguration(const TwitterConfiguration &other)
{
    obj_ = other.obj_;

    sizeThumb_ = other.sizeThumb_;
    sizeLarge_ = other.sizeLarge_;
    sizeMedium_ = other.sizeMedium_;
    sizeSmall_ = other.sizeSmall_;
}

int TwitterConfiguration::dmTextCharacterLimit()
{
    return obj_.value("dm_text_character_limit").toInt();
}

int TwitterConfiguration::charactersReservedPerMedia()
{
    return obj_.value("characters_reserved_per_media").toInt();
}

int TwitterConfiguration::maxMediaPerUpload()
{
    return obj_.value("max_media_per_upload").toInt();
}

QVariantList TwitterConfiguration::nonUsernamePaths()
{
    return obj_.value("non_username_paths").toArray().toVariantList();
}

int TwitterConfiguration::photoSizeLimit()
{
    return obj_.value("photo_size_limit").toInt();
}

int TwitterConfiguration::shortUrlLength()
{
    return obj_.value("short_url_length").toInt();
}

int TwitterConfiguration::shortUrlLengthHttps()
{
    return obj_.value("short_url_length_https").toInt();
}

TwiiterMediaSize TwitterConfiguration::sizeThumb() const
{
    return sizeThumb_;
}

TwiiterMediaSize TwitterConfiguration::sizeLarge() const
{
    return sizeLarge_;
}

TwiiterMediaSize TwitterConfiguration::sizeMedium() const
{
    return sizeMedium_;
}

TwiiterMediaSize TwitterConfiguration::sizeSmall() const
{
    return sizeSmall_;
}
