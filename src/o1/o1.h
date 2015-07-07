#ifndef O1_H
#define O1_H

#include <QObject>
#include <SynchronousHttpRequest/synchronoushttprequest.h>
#include <SqlSettings/sqlsettings.h>
#include <QTcpServer>
#include <QTimer>
#include <QThread>

class O1 : public QObject
{
    Q_OBJECT
public:
    explicit O1(const QString &requestTokenURL, const QString &authorizeURL, const QString &accessTokenURL, const QString &consumerKey, const QString &consumerSecret, SqlSettings *settings, QObject *parent = 0);
    ~O1();

    bool login();
    bool logedIn();

    QString requestTokenURL() const;
    void setRequestTokenURL(const QString &requestTokenURL);
    QString authorizeURL() const;
    void setAuthorizeURL(const QString &authorizeURL);
    QString accessTokenURL() const;
    void setAccessTokenURL(const QString &accessTokenURL);
    QString consumerKey() const;
    void setConsumerKey(const QString &consumerKey);
    QString consumerSecret() const;
    void setConsumerSecret(const QString &consumerSecret);
    QString getOAuthToken() const;
    QString getOAuthVerifier() const;
    QString getConsumerKey() const;
    QByteArray nonce();

    QString sign(QString httpMethod, QString url, QMap<QString, QString> params);
    QByteArray buildAuthorizationHeader(QMap<QString, QString> headerParameters);

private:
    QString requestTokenURL_;
    QString authorizeURL_;
    QString accessTokenURL_;
    QString consumerKey_;
    QString consumerSecret_;
    QString oAuthToken_;
    QString oAuthVerifier_;
    QString oAuthTokenSecret_;
    SqlSettings* settings_;

    SynchronousHttpRequest* shr_;

    QMap<QString, QString> parseResponse(const QByteArray &response);
    void requestToken();
    void authorize();
    void accessToken();

    //HTTP Server
    QTcpServer server_;
    QEventLoop serverLoop_;
    QTimer serverTimer_;
    //quint16 port_;
    QMap<QString, QString> queryParams_;

    void initServer();
    QMap<QString, QString> startServer();

private slots:
    void onIncomingConnection();
    void onTimeOut();
    void onBytesReady();
    QMap<QString, QString> parseQueryParams(const QByteArray &data);

};

#endif // O1_H
