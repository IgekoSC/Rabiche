#include "twittersymbol.h"

TwitterSymbol::TwitterSymbol(const QJsonObject &jsonObj) :
    TwitterEntity(jsonObj)
{

}

TwitterSymbol::TwitterSymbol(const TwitterSymbol &other) :
    TwitterEntity(other)
{

}

TwitterSymbol::~TwitterSymbol()
{

}

QString TwitterSymbol::text()
{
    return obj_.value("text").toString();
}
