#include "twitterstatus.h"

TwitterStatus::TwitterStatus()
{

}

TwitterStatus::TwitterStatus(const TwitterStatus &other)
{
    params_ = other.params_;
}

TwitterStatus::~TwitterStatus()
{

}

QString TwitterStatus::status() const
{
    return params_.value("status");
}

void TwitterStatus::setStatus(const QString &status)
{
    params_.insert("status", status);
}

qint64 TwitterStatus::inReplyToStatusId() const
{
    return params_.value("in_reply_to_status_id").toLongLong();
}

void TwitterStatus::setInReplyToStatusId(const qint64 &inReplyToStatusId)
{
    params_.insert("in_reply_to_status_id", QString::number(inReplyToStatusId));
}

bool TwitterStatus::possiblySensitive() const
{
    return stringToBool(params_.value("possibly_sensitive"));
}

void TwitterStatus::setPossiblySensitive(bool possiblySensitive)
{
    params_.insert("possibly_sensitive", boolToString(possiblySensitive));
}

double TwitterStatus::latitude() const
{
    return params_.value("lat").toDouble();
}

void TwitterStatus::setLatitude(double latitude)
{
    params_.insert("lat", QString::number(latitude));
}

double TwitterStatus::longitude() const
{
    return params_.value("long").toDouble();
}

void TwitterStatus::setLongitude(double longitude)
{
    params_.insert("long", QString::number(longitude));
}

QString TwitterStatus::placeId() const
{
    return params_.value("place_id");
}

void TwitterStatus::setPlaceId(const QString &placeId)
{
    params_.insert("place_id", placeId);
}

bool TwitterStatus::displayCoordinates() const
{
    return stringToBool(params_.value("display_coordinates"));
}

void TwitterStatus::setDisplayCoordinates(bool displayCoordinates)
{
    params_.insert("display_coordinates", boolToString(displayCoordinates));
}

bool TwitterStatus::trimUser() const
{
    return stringToBool(params_.value("trim_user"));
}

void TwitterStatus::setTrimUser(bool trimUser)
{
    params_.insert("trim_user", boolToString(trimUser));
}

QString TwitterStatus::mediaIds() const
{
    return params_.value("media_ids");
}

void TwitterStatus::setMediaIds(const QString &mediaIds)
{
    params_.insert("media_ids", mediaIds);
}

QMap<QString, QString> TwitterStatus::params()
{
    return params_;
}

bool TwitterStatus::stringToBool(const QString &value) const
{
    return (value.toLower() == "true");
}

QString TwitterStatus::boolToString(const bool &value) const
{
    if (value)
        return "true";

    return "false";
}
