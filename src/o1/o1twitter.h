#ifndef O1TWITTER_H
#define O1TWITTER_H

#include "o1.h"

class O1Twitter : public O1
{
    Q_OBJECT
public:
    O1Twitter(const QString &consumerKey, const QString &consumerSecret, SqlSettings *settings, QObject *parent = 0);
    ~O1Twitter();
};

#endif // O1TWITTER_H
