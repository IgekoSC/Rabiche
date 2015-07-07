#include "signedrequester.h"

SignedRequester::SignedRequester(O1 *o1, QObject *parent) : QObject(parent), o1_(o1)
{

}

QByteArray SignedRequester::buildAuthorizationHeader(QString url, QMap<QString, QString> params, QString verb)
{
    QMap<QString, QString> headerParameters;
    headerParameters.insert("oauth_token", o1_->getOAuthToken());
    headerParameters.insert("oauth_verifier", o1_->getOAuthVerifier());
    headerParameters.insert("oauth_consumer_key", o1_->getConsumerKey());
    headerParameters.insert("oauth_nonce", o1_->nonce());
    headerParameters.insert("oauth_timestamp", QString::number(QDateTime::currentDateTimeUtc().toTime_t()));
    headerParameters.insert("oauth_version", "1.0");
    headerParameters.insert("oauth_signature_method", "HMAC-SHA1");

    //Generate signature parameters
    QMap<QString, QString> signatureParameters;
    QStringList keys = headerParameters.keys();
    foreach (QString key, keys) {
        signatureParameters.insert(key, headerParameters[key]);
    }
    keys = params.keys();
    foreach (QString key, keys) {
        signatureParameters.insert(key, params[key]);
    }

    //Sign
    headerParameters.insert("oauth_signature", o1_->sign(verb, url, signatureParameters));

    return o1_->buildAuthorizationHeader(headerParameters);
}

QByteArray SignedRequester::doAuthorizedRequest(SynchronousHttpRequest* shr, QString url, QMap<QString, QString> params, QString verb)
{
    QString paramsStr;
    if ((params.size() > 0) && (verb == "GET")) {
        QStringList keys = params.keys();
        bool first = true;
        foreach (QString key, keys) {
            QString param = params[key];
            if (first)
                paramsStr += "?" + key + "=" + param;
            else
                paramsStr += "&" + key + "=" + param;
            first = false;
        }
    }

    shr->reset();
    shr->setRawHeader("Authorization", buildAuthorizationHeader(url, params, verb));

    if (verb == "POST") {
        return shr->post(url, params);
    } else {
        return shr->get(url + paramsStr);
    }
}
