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

QJsonArray Rabiche::tweets()
{
    QJsonArray tweets = newTweets_;
    newTweets_ = QJsonArray();

    return tweets;
}

const QJsonArray &Rabiche::allTweets()
{
    return allTweets_;
}

void Rabiche::onNewTweets(TweetsMap tweets)
{
    foreach (Tweet tweet, tweets) {
        allTweets_.push_front(tweet.jsonObj());
        newTweets_.push_front(tweet.jsonObj());
    }

    emit newTweets();
}
