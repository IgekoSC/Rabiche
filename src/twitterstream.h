#ifndef TWITTERSTREAM_H
#define TWITTERSTREAM_H

#include <QObject>
#include <QNetworkAccessManager>
#include <o1/signedrequester.h>
#include <QMutexLocker>
#include <QJsonDocument>
#include <QJsonObject>
#include <tweet.h>

class TwitterStream : public QObject
{
    Q_OBJECT
public:
    explicit TwitterStream(O1 *o1, QObject *parent = 0);
    QList<QJsonObject> getJsonObjects();
    TweetsMap getTweets();

signals:
    void newObjects();
    void newTweets();
    void finished();

public slots:
    bool start(QString url, QMap<QString, QString> params = QMap<QString, QString>(), QString verb = "GET");
    bool stop();

private slots:
    void onReadyRead();
    void onFinished();

private:
    O1 *o1_;
    SignedRequester* req_;
    bool running_;
    QNetworkReply *networkReply_;
    QNetworkRequest networkRequest_;
    QNetworkAccessManager networkAccessManager_;
    QMutex mutex_;
    QByteArray buffer_;
    QList<QJsonObject> objects_;
    TweetsMap tweets_;

    void init();
    void makeConnections();
    void processBuffer();
    void get(const QString &url, const QMap<QString, QString> &params);
    void post(const QString &url, const QMap<QString, QString> &parameters);
};

#endif // TWITTERSTREAM_H
