#include "o1twitter.h"

#define REQUEST_TOKEN_URL "https://api.twitter.com/oauth/request_token"
#define AUTHORIZE_URL     "https://api.twitter.com/oauth/authorize"
#define ACCESS_TOKEN_URL  "https://api.twitter.com/oauth/access_token"


O1Twitter::O1Twitter(const QString &consumerKey, const QString &consumerSecret, SqlSettings *settings, QObject *parent) :
    O1(REQUEST_TOKEN_URL, AUTHORIZE_URL, ACCESS_TOKEN_URL, consumerKey, consumerSecret, settings, parent)
{

}

O1Twitter::~O1Twitter()
{

}


