#include <QDebug>
#include <QCoreApplication>
#include "twitter.h"

#define traceDebug() qDebug() << QThread::currentThreadId() << __PRETTY_FUNCTION__

Twitter::Twitter(QString name, QObject *parent) :
    QObject(parent)
{
    qRegisterMetaType<TweetsMap>("TweetsMap");

    setObjectName(name);

    settings_ = 0;
    shr_      = 0;
    auth_     = 0;
    req_      = 0;
    stream_   = 0;

    loginState_            = 0;
    streamConnectionState_ = 0;

    refreshTimer_ = 0;
    lastRefreshedTweetId_ = 0;
}

Twitter::~Twitter()
{
    loguout();
}

const TweetsMap &Twitter::tweets()
{
    QMutexLocker locker(&mutex_);

    return tweets_;
}

const TweetsMap &Twitter::homeTimeline(qint64 max_id, qint64 since_id, int count)
{
    QMutexLocker locker(&mutex_);

    QMap<QString, QString> params;
    if (max_id > 0) {
        params.insert("max_id", QString::number(max_id));
    }
    if (since_id > 0) {
        params.insert("since_id", QString::number(since_id));
    }
    params.insert("count", QString::number(count));

    QJsonArray jsonTweets = QJsonDocument::fromJson(req_->doAuthorizedRequest(shr_, "https://api.twitter.com/1.1/statuses/home_timeline.json", params)).array();

    TweetsMap tweets;
    foreach (QJsonValue jsonTweet, jsonTweets) {
        Tweet tweet(jsonTweet.toObject());
        tweets.insert(tweet.id(), tweet);
        tweets_.insert(tweet.id(), tweet);
    }

    updateTweetsDb(tweets);

    return tweets_;
}

const qint64 &Twitter::oldest_id()
{
    QMutexLocker locker(&mutex_);

    return oldest_id_;
}

const qint64 &Twitter::newest_id()
{
    QMutexLocker locker(&mutex_);

    return newest_id_;
}

void Twitter::login()
{
    if (this->thread() != QThread::currentThread()) {
        traceDebug() << "Warning: This function has been called from diferent thread!" << flush;
    }

    //Logout
    loguout();

    QMutexLocker locker(&mutex_);

    //Initialize http requester and refresh timer
    if (shr_ == 0)
        shr_  = new SynchronousHttpRequest(this);
    if (refreshTimer_ == 0) {
        refreshTimer_ = new QTimer(this);
        connect(refreshTimer_, SIGNAL(timeout()), this, SLOT(onRefreshTimerTimeout()));
    }

    //Setup database
    ConecToDb(db_, "rabiche_" + objectName() + "_db");
    settings_ = new SqlSettings(&db_, this);
    db_.exec("CREATE TABLE IF NOT EXISTS tweets ("
              "id TEXT NOT NULL,"
              "json TEXT NOT NULL"
              ");");
    db_.exec("CREATE UNIQUE INDEX IF NOT EXISTS \"Ids\" ON tweets (id DESC);");

    //Auth
    auth_ = new O1Twitter("qv9DUeDWumhUydA2TDnj7zPk3", "UO5zz7oVRvrs9USUF7HU52usdqXpYH5Wp6Zs4MelSPTQYtNp8a", settings_, this);
    req_  = new SignedRequester(auth_, this);

    //Recover all tweets from cache
    QSqlQuery query = db_.exec("SELECT * FROM tweets;");
    while (query.next()) {
        Tweet tweet(QJsonDocument::fromJson(GetField(query, "json").toByteArray()).object());
        tweets_.insert(tweet.id(), tweet);
    }
    if (tweets_.size() > 0) {
        newest_id_  = tweets_.last().id() + 1;
        oldest_id_ = tweets_.first().id();
        lastRefreshedTweetId_ = tweets_.first().id();
    } else {
        newest_id_  = 0;
        oldest_id_ = 0;
    }
    traceDebug() << QString::number(tweets_.size()) + " twitts loaded." << flush;

    //Initialize stream listener
    stream_ = new TwitterStream(auth_, this);
    connect(stream_, SIGNAL(newTweets()), this, SLOT(onStreamNewTweets()), Qt::QueuedConnection);

    //Login
    if (auth_->login())
        loginState_ = 1;
    else
        loginState_ = 255;

    //Start timer for continuous refreshing from REST
    refreshTimer_->start(60000); //1 min.
}

void Twitter::loguout()
{
    QMutexLocker locker(&mutex_);

    if (settings_ != 0)
        settings_->deleteLater();
    if (auth_ != 0)
        auth_->deleteLater();
    if (req_ != 0)
        req_->deleteLater();
    if (stream_ != 0) {
        stream_->stop();
        stream_->deleteLater();
    }
    if (refreshTimer_ != 0)
        refreshTimer_->stop();

    loginState_ = 0;
    streamConnectionState_ = 0;

    tweets_.clear();

    lastRefreshedTweetId_ = 0;
}

void Twitter::connectToStream(QString uri)
{
    if (this->thread() != QThread::currentThread()) {
        traceDebug() << "Warning: This function has been called from diferent thread!" << flush;
    }

    QMutexLocker locker(&mutex_);

    if (stream_->start(uri))
        streamConnectionState_ = 1;
    else
        streamConnectionState_ = 255;
}

void Twitter::onStreamNewTweets()
{
    QMutexLocker locker(&mutex_);

    TweetsMap tweets = stream_->getTweets();

    foreach (Tweet tweet, tweets) {
        tweets_.insert(tweet.id(), tweet);
    }

    if (!tweets_.empty()) {
        newest_id_  = tweets_.first().id() + 1;
        oldest_id_ = tweets_.last().id();
    }

    //Store all tweets to db
    updateTweetsDb(tweets);

    if (lastRefreshedTweetId_ == 0)
        lastRefreshedTweetId_ = tweets_.first().id();

    emit newTweets(tweets);
}

void Twitter::onRefreshTimerTimeout()
{
    QMutexLocker locker(&mutex_);

    //Get up to 6 x 100 tweets to update status
    TweetsMap tweets;
    TweetsMap::iterator it = tweets_.find(lastRefreshedTweetId_);
    for (int i = 0; (i < 6) && (lastRefreshedTweetId_ != tweets_.last().id()); ++i) {
        //Get up to 100 tweets to update status
        QStringList ids;
        for (int j = 0; (j < 100) && (it != tweets_.end()); ++j) {
            ids.push_back(it.value().idStr());
            it++;
        }
        lastRefreshedTweetId_ = ids.last().toLongLong();
        //Request for new data
        QMap<QString, QString> params;
        params.insert("id", ids.join(","));
        QJsonArray arr = QJsonDocument::fromJson(req_->doAuthorizedRequest(shr_, TWITTER_REST_LOOKUP_URI, params)).array();
        foreach (QJsonValue val, arr) {
            Tweet tweet(val.toObject());
            tweets.insert(tweet.id(), tweet);
        }
    }

    if (lastRefreshedTweetId_ == tweets_.last().id()) {
        lastRefreshedTweetId_ = tweets_.first().id();
    }

    updateTweetsDb(tweets);

    emit newTweets(tweets);
}

unsigned char Twitter::streamConnectionState()
{
    QMutexLocker locker(&mutex_);
    return streamConnectionState_;
}

unsigned char Twitter::loginState()
{
    QMutexLocker locker(&mutex_);
    return loginState_;
}

void Twitter::updateTweetsDb(const TweetsMap &tweets)
{
    if (tweets.size() == 0)
        return;
    //Construct query
    QString query = "INSERT OR REPLACE INTO 'tweets' SELECT ? AS 'id', ? AS 'json'";
    for (int i = 1; i < tweets.size(); ++i){
        query += " UNION SELECT ?, ?";
    }
    //Bind values
    QSqlQuery q(db_);
    q.prepare(query);
    foreach (Tweet tweet, tweets) {
        q.addBindValue(tweet.idStr());
        q.addBindValue(QJsonDocument(tweet.jsonObj()).toJson());
    }
    //Exec query and check for errors
    q.exec();
    if (q.lastError().type() != QSqlError::NoError) {
        traceDebug() << "Error: " << q.lastQuery() << q.lastError().text();
    }
}
