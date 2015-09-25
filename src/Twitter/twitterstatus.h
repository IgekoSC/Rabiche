#ifndef TWITTERSTATUS_H
#define TWITTERSTATUS_H

#include <QJsonObject>
#include <QJsonArray>
#include <QList>
#include <QMap>

class TwitterStatus
{
public:
    TwitterStatus();
    TwitterStatus(const TwitterStatus &other);
    ~TwitterStatus();

    QString status() const;
    void setStatus(const QString &status);
    qint64 inReplyToStatusId() const;
    void setInReplyToStatusId(const qint64 &inReplyToStatusId);
    bool possiblySensitive() const;
    void setPossiblySensitive(bool possiblySensitive);
    double latitude() const;
    void setLatitude(double latitude);
    double longitude() const;
    void setLongitude(double longitude);
    QString placeId() const;
    void setPlaceId(const QString &placeId);
    bool displayCoordinates() const;
    void setDisplayCoordinates(bool displayCoordinates);
    bool trimUser() const;
    void setTrimUser(bool trimUser);
    QString mediaIds() const;
    void setMediaIds(const QString &mediaIds);

    QMap<QString, QString> params();

private:
    QMap<QString, QString> params_;
    bool stringToBool(const QString& value) const;
    QString boolToString(const bool& value) const;

};



#endif // TWITTERSTATUS_H
