#include "synchronoushttprequest.h"
#include <QBuffer>
#include <QDebug>
#include <QThread>

#define traceDebug() qDebug() << QThread::currentThreadId() << __PRETTY_FUNCTION__

SynchronousHttpRequest::SynchronousHttpRequest(QObject *parent) :
    QObject(parent)
{
    toFile_ = 0;
    fromFile_ = 0;
    multiPart_ = 0;
}

SynchronousHttpRequest::~SynchronousHttpRequest()
{

}

void SynchronousHttpRequest::reset()
{
    networkRequest_ = QNetworkRequest(QUrl());
}

void SynchronousHttpRequest::setRawHeader(const QByteArray &headerName, const QByteArray &headerValue)
{
    networkRequest_.setRawHeader(headerName, headerValue);
}

void SynchronousHttpRequest::setHeader(QNetworkRequest::KnownHeaders header, const QVariant &value)
{
    networkRequest_.setHeader(header, value);
}

const QByteArray &SynchronousHttpRequest::post(const QUrl &url, const QMap<QString, QString> &parameters)
{
    init();

    QByteArray body;
    bool first = true;
    foreach (QString key, parameters.keys()) {
        if (first) {
            first = false;
        } else {
            body.append("&");
        }
        QString value = parameters.value(key);
        body.append(QUrl::toPercentEncoding(key) + QString("=").toUtf8() + QUrl::toPercentEncoding(value));
    }

    networkRequest_.setUrl(url);
    networkReply_ = networkAccessManager_.post(networkRequest_, body);

    makeConnections();

    loop_.exec();

    return buffer_;
}

const QByteArray &SynchronousHttpRequest::post(const QUrl &url, const QByteArray &data)
{
    init();

    networkRequest_.setUrl(url);

    networkReply_ = networkAccessManager_.post(networkRequest_, data);

    makeConnections();

    loop_.exec();

    return buffer_;
}

const QByteArray &SynchronousHttpRequest::post(const QUrl &url, const QMap<QString, QString> &parameters, QFile *fromFile)
{
    init();

    fromFile_ = new QFile(fromFile->fileName());
    fromFile_->open(QIODevice::ReadOnly);

    multiPart_ = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    foreach (QString key, parameters.keys()) {
        QHttpPart part;
        part.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\""+key+"\""));
        part.setBody(parameters[key].toUtf8());
        multiPart_->append(part);
    }

    QFileInfo fi(fromFile_->fileName());
    QMimeDatabase mimedb;

    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"file\"; filename=\""+fromFile_->fileName().section('/',-1)+"\""));
    filePart.setHeader(QNetworkRequest::ContentLengthHeader, QString("%1").arg(fi.size()));
    filePart.setHeader(QNetworkRequest::ContentTypeHeader, mimedb.mimeTypeForFile(fi).name());
    filePart.setRawHeader("Content-Transfer-Encoding", "binary");
    filePart.setBodyDevice(fromFile_);
    multiPart_->append(filePart);

    networkRequest_.setUrl(url);

    networkReply_ = networkAccessManager_.post(networkRequest_, multiPart_);

    makeConnections();

    loop_.exec();

    return buffer_;
}

const QByteArray &SynchronousHttpRequest::get(const QUrl &url, QFile *toFile)
{
    if (toFile != 0) {
        toFile_ = new QFile(toFile->fileName());
        toFile_->open(QIODevice::WriteOnly | QIODevice::Truncate);
    } else {
        toFile_ = 0;
    }

    init();

    networkRequest_.setUrl(url);
    networkReply_ = networkAccessManager_.get(networkRequest_);

    makeConnections();

    loop_.exec();

    return buffer_;
}

const QByteArray &SynchronousHttpRequest::put(const QUrl &url, QFile *fromFile, qint64 chunkIndex, qint64 chunkSize)
{
    fromFile_ = new QFile(fromFile->fileName());
    fromFile_->open(QIODevice::ReadOnly);

    init();

    networkRequest_.setUrl(url);

    if (chunkIndex != -1) {
        fromFile_->seek(chunkIndex * chunkSize);
        QByteArray chunk = fromFile_->read(chunkSize);
        networkReply_ = networkAccessManager_.put(networkRequest_, chunk);
    } else {
        networkReply_ = networkAccessManager_.put(networkRequest_, fromFile_);
    }

    makeConnections();

    loop_.exec();

    return buffer_;
}

const QByteArray &SynchronousHttpRequest::sendCustomRequest(const QByteArray &verb, const QUrl &url, QByteArray data)
{
    init();

    networkRequest_.setUrl(url);
    QBuffer buffer(&data);
    networkReply_ = networkAccessManager_.sendCustomRequest(networkRequest_, verb, &buffer);

    makeConnections();

    loop_.exec();

    return buffer_;
}

const QList<QNetworkReply::RawHeaderPair> &SynchronousHttpRequest::getLastReplyRawHeaders()
{
    return lastReplyRawHeaders_;
}

const QNetworkReply::NetworkError &SynchronousHttpRequest::getLastError()
{
    return lastError_;
}

const QString &SynchronousHttpRequest::getLastErrorString()
{
    return lastErrorString_;
}

void SynchronousHttpRequest::init()
{
    buffer_.clear();
    lastError_ = QNetworkReply::NoError;
//    QVariant header = networkRequest_.header(QNetworkRequest::ContentTypeHeader);
//    if (header == QVariant::Invalid)
    //        networkRequest_.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
}

void SynchronousHttpRequest::makeConnections()
{
    connect(networkReply_, SIGNAL(finished()), this, SLOT(onRequestFinished()));
    connect(networkReply_, SIGNAL(finished()), this, SIGNAL(finished()));
    connect(networkReply_, SIGNAL(readyRead()), this, SLOT(onReplyReadyRead()));
    connect(networkReply_, SIGNAL(readyRead()), this, SIGNAL(readyRead()));
    connect(networkReply_, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onReplyError(QNetworkReply::NetworkError)));
    connect(networkReply_, SIGNAL(downloadProgress(qint64,qint64)), this, SIGNAL(downloadProgress(qint64,qint64)));
    connect(networkReply_, SIGNAL(uploadProgress(qint64,qint64)), this, SIGNAL(uploadProgress(qint64,qint64)));
}

void SynchronousHttpRequest::onRequestFinished()
{
    //traceDebug() << "Request Finished" << endl;

    if (toFile_ != 0) {
        toFile_->close();
        delete toFile_;
        toFile_ = 0;
    }

    if (fromFile_ != 0) {
        fromFile_->close();
        delete fromFile_;
        fromFile_ = 0;
    }

    if (multiPart_ != 0) {
        delete multiPart_;
        multiPart_ = 0;
    }

    loop_.quit();

    lastReplyRawHeaders_ = networkReply_->rawHeaderPairs();

    emit finished();

    //networkReply_->deleteLater();
}

void SynchronousHttpRequest::onReplyReadyRead()
{
    //traceDebug() << "Reading " << networkReply_->bytesAvailable() << "bytes." << endl;
    if (toFile_ == 0) {
        buffer_.append(networkReply_->readAll());
    } else {
        toFile_->write(networkReply_->readAll());
    }

    emit readyRead();
}

void SynchronousHttpRequest::onReplyError(QNetworkReply::NetworkError err)
{
    traceDebug() << "NetworkError: " << networkReply_->errorString() << endl;
    lastError_ = err;
    lastErrorString_ = networkReply_->errorString();
    emit ReplyError(networkReply_->errorString());
}
