#ifndef SYNCHRONOUSNETWORKREQUEST_H
#define SYNCHRONOUSNETWORKREQUEST_H

#include <QObject>
#include <QUrlQuery>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QFile>
#include <QHttpPart>
#include <QMimeDatabase>
#include <QFileInfo>
#include <QMutexLocker>

class SynchronousHttpRequest : public QObject
{
    Q_OBJECT
public:
    explicit SynchronousHttpRequest(QObject *parent = 0);
    ~SynchronousHttpRequest();
    void reset();
    void setRawHeader(const QByteArray & headerName, const QByteArray & headerValue);
    void setHeader(QNetworkRequest::KnownHeaders header, const QVariant & value);
    const QByteArray& post(const QUrl& url, const QMap<QString, QString> &parameters);
    const QByteArray& post(const QUrl& url, const QByteArray &data);
    const QByteArray& post(const QUrl& url, const QMap<QString, QString> &parameters, QFile *fromFile);
    const QByteArray& get(const QUrl& url, QFile *toFile = 0);
    const QByteArray& put(const QUrl& url, QFile *fromFile, qint64 chunkIndex = -1, qint64 chunkSize = 10485760);
    const QByteArray& sendCustomRequest(const QByteArray & verb, const QUrl& url, QByteArray data = QByteArray());
    const QList<QNetworkReply::RawHeaderPair> &getLastReplyRawHeaders();
    const QNetworkReply::NetworkError& getLastError();
    const QString& getLastErrorString();

private:
    QNetworkReply *networkReply_;
    QNetworkRequest networkRequest_;
    QHttpMultiPart *multiPart_;
    QEventLoop loop_;
    QNetworkAccessManager networkAccessManager_;
    QNetworkReply::NetworkError lastError_;
    QString lastErrorString_;
    QByteArray buffer_;
    QList<QNetworkReply::RawHeaderPair> lastReplyRawHeaders_;
    QFile* toFile_;
    QFile* fromFile_;

    void init();
    inline void makeConnections();

signals:
    void ReplyError(QString err);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void uploadProgress(qint64 bytesSent, qint64 bytesTotal);
    void finished();
    void readyRead();

private slots:
    void onRequestFinished();
    void onReplyReadyRead();
    void onReplyError(QNetworkReply::NetworkError err);

};

#endif // SYNCHRONOUSNETWORKREQUEST_H
