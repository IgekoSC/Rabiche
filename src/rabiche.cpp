#include "rabiche.h"

Rabiche::Rabiche(QObject *parent) : QObject(parent)
{
    thread_ = new QThread(this);
    thread_->start();
    while (!thread_->isRunning())
        QThread::msleep(1);

    twitter_ = new Twitter("eliasrm87");
    twitter_->moveToThread(thread_);

    connect(twitter_, SIGNAL(newTweets(TweetsMap)), this, SLOT(onNewTweets(TweetsMap)));
    connect(twitter_, SIGNAL(newOfflineTweets(TweetsMap)), this, SLOT(onNewOfflineTweets(TweetsMap)));

    QMetaObject::invokeMethod(twitter_, "login");

    while (twitter_->loginState() == 0)
        QThread::msleep(1);

    if (twitter_->loginState() != 1) {
        qDebug() << "Login fail: " << twitter_->loginState() << flush;
        return;
    }

    onNewTweets(twitter_->tweets());

    QMetaObject::invokeMethod(twitter_, "connectToStream", Q_ARG(QString, TWITTER_USER_STREAM_URI));
}

Rabiche::~Rabiche()
{

}

void Rabiche::end()
{
    twitter_->loguout();

    thread_->quit();
    while (thread_->isRunning())
        QThread::msleep(1);

    twitter_->deleteLater();
}

QJsonArray Rabiche::getNewTweets()
{
    QJsonArray tweets = newTweets_;
    newTweets_ = QJsonArray();

    return tweets;
}

QJsonArray Rabiche::getNewOfflineTweets()
{
    QJsonArray tweets = offlineTweets_;
    offlineTweets_ = QJsonArray();

    return tweets;
}

void Rabiche::onNewTweets(TweetsMap tweets)
{
    foreach (Tweet tweet, tweets) {
        newTweets_.push_front(tweet.jsonObj());
    }

    emit newTweets();
}

void Rabiche::onNewOfflineTweets(TweetsMap tweets)
{
    foreach (Tweet tweet, tweets) {
        offlineTweets_.push_front(tweet.jsonObj());
    }

    emit newOfflineTweets();
}
