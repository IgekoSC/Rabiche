#ifndef TWITTERSYMBOL_H
#define TWITTERSYMBOL_H

#include <QJsonObject>
#include <QList>
#include <QJsonArray>
#include "twitterentity.h"

class TwitterSymbol : public TwitterEntity
{
public:
    TwitterSymbol(const QJsonObject &jsonObj);
    TwitterSymbol(const TwitterSymbol &other);
    ~TwitterSymbol();

    QString text();

};

#endif // TWITTERSYMBOL_H
