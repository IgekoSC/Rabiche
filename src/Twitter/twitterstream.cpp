#include "twitterstream.h"
#include <QDebug>

#define traceDebug() qDebug() << QThread::currentThreadId() << __PRETTY_FUNCTION__

TwitterStream::TwitterStream(O1 *o1, QObject *parent) :
    QObject(parent), o1_(o1)
{
    req_ = new SignedRequester(o1_, this);
    running_ = false;
}

QList<QJsonObject> TwitterStream::getJsonObjects()
{
    QMutexLocker locker(&mutex_);

    QList<QJsonObject> objs = objects_;
    objects_.clear();

    return objs;
}

TweetsMap TwitterStream::getTweets()
{
    QMutexLocker locker(&mutex_);

    TweetsMap tweets = tweets_;
    tweets_.clear();

    return tweets;
}

bool TwitterStream::start(QString url, QMap<QString, QString> params, QString verb)
{
    QMutexLocker locker(&mutex_);

    if (running_)
        return false;

    if (verb == "GET") {
        get(url, params);
    } else {
        post(url, params);
    }

    running_ = true;

    return true;
}

bool TwitterStream::stop()
{
    QMutexLocker locker(&mutex_);

    if (!running_)
        return false;

    running_ = false;

    networkReply_->close();

    while (!networkReply_->isFinished())
        QThread::msleep(1);

    return true;
}

void TwitterStream::onReadyRead()
{
    QMutexLocker locker(&mutex_);

    buffer_.append(networkReply_->readAll());

    processBuffer();
}

void TwitterStream::onFinished()
{
//    QMutexLocker locker(&mutex_);
    traceDebug();

    running_ = false;

    emit finished();
}

void TwitterStream::init()
{
    buffer_.clear();
}

void TwitterStream::makeConnections()
{
    disconnect(this);
    connect(networkReply_, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(networkReply_, SIGNAL(finished()), this, SLOT(onFinished()));
}

void TwitterStream::processBuffer()
{
    int cStart = 0;
    int cEnd   = 0;

    bool bNewTweets = false;
    bool bNewObjects = false;

    int i = 0;
    while (i < buffer_.size()) {
        if (buffer_[i] == '{') {
            cStart++;
            if (cStart == 1) {
                buffer_.remove(0, i);
                i = 0;
            }
        } else if (buffer_[i] == '}') {
            cEnd++;
            if (cStart == cEnd) {
                QJsonObject obj = QJsonDocument::fromJson(buffer_.mid(0, i+1)).object();

                if (obj.contains("text")) {
                    Tweet tweet(obj);
                    tweets_.insert(tweet.id(), tweet);
                    bNewTweets = true;
                } else {
                    objects_.push_front(obj);
                    bNewObjects = true;
                    qDebug() << obj << endl;
                }

                buffer_.remove(0, i+1);
                i = 0;
                cStart = 0;
                cEnd   = 0;
            }
        }
        ++i;
    }

    if (bNewTweets)
        emit newTweets();
    if (bNewObjects)
        emit newObjects();
}

void TwitterStream::get(const QString &url, const QMap<QString, QString> &params)
{
    QString paramsStr;
    if (params.size() > 0) {
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

    init();

    networkRequest_ = QNetworkRequest(url + paramsStr);
    networkRequest_.setRawHeader("Authorization", req_->buildAuthorizationHeader(url, params));

    networkReply_ = networkAccessManager_.get(networkRequest_);

    makeConnections();
}

void TwitterStream::post(const QString &url, const QMap<QString, QString> &params)
{
    QByteArray body;
    bool first = true;
    foreach (QString key, params.keys()) {
        if (first) {
            first = false;
        } else {
            body.append("&");
        }
        QString value = params.value(key);
        body.append(QUrl::toPercentEncoding(key) + QString("=").toUtf8() + QUrl::toPercentEncoding(value));
    }

    init();

    networkRequest_ = QNetworkRequest(url);
    networkRequest_.setRawHeader("Authorization", req_->buildAuthorizationHeader(url, params));
    networkReply_ = networkAccessManager_.post(networkRequest_, body);

    makeConnections();
}
