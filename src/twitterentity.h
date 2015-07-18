#ifndef TWITTERENTITY_H
#define TWITTERENTITY_H

#include <QJsonObject>
#include <QList>
#include <QJsonArray>

class TwitterEntity
{
public:
    TwitterEntity(const QJsonObject &jsonObj);
    TwitterEntity(const TwitterEntity &other);
    ~TwitterEntity();

    const QList<int> &indices();
    const QJsonObject &jsonObj();

protected:
    QJsonObject obj_;
    QList<int> indices_;

};

#endif // TWITTERENTITY_H
