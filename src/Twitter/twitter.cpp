#include <QDebug>
#include <QCoreApplication>
#include <QDateTime>
#include "twitter.h"

#define traceDebug() qDebug() << QThread::currentThreadId() << __PRETTY_FUNCTION__

Twitter::Twitter(QString name, QObject *parent) :
    QObject(parent)
{
    qRegisterMetaType<TweetsMap>("TweetsMap");
    qRegisterMetaType<TwitterStatus>("TwitterStatus");

    setObjectName(name);

    settings_ = 0;
    shr_      = 0;
    auth_     = 0;
    req_      = 0;
    stream_   = 0;

    loginState_            = 0;
    streamConnectionState_ = 0;

    refreshTimer_ = new QTimer(this);
    connect(refreshTimer_, SIGNAL(timeout()), this, SLOT(onRefreshTimerTimeout()));

    lastRefreshedTweetId_ = 0;

    oldest_id_ = 0;
    newest_id_ = 0;

    currentPage_ = 0;
    pageSize_ = 0;
}

Twitter::~Twitter()
{
    refreshTimer_->stop();
    loguout();
    refreshTimer_->deleteLater();
}

const TweetsMap &Twitter::tweets(int page, int pageSize)
{
    QMutexLocker locker(&mutex_);

    currentPage_ = page;
    pageSize_ = pageSize;

    pageTweets_.clear();

    if (tweets_.size() == 0) {
        currentPage_ = 0;
        return tweets_;
    }

    if (pageSize == 0) {
        currentPage_ = 0;
        return tweets_;
    } else {
        int nPages = tweets_.size() / pageSize;
        //Check for not valid page
        if (nPages == 0) {
            page = 0;
            currentPage_ = 0;
        }
        //Get page items
        QMapIterator<qint64, Tweet> i(tweets_);
        i.toBack();
        int c = 0;
        int nPage = 0;
        while (i.hasPrevious() && (nPage <= page)) {
            i.previous();
            if (nPage == page) {
                Tweet tweet(i.value());
                pageTweets_.insert(tweet.id(), tweet);
            }
            c++;
            if ((c%pageSize) == 0) {
                nPage++;
            }
        }

        return pageTweets_;
    }
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

    //Setup database
    ConecToDb(db_, "rabiche_" + objectName() + "_db");
    settings_ = new SqlSettings(&db_, this);
    db_.exec("CREATE TABLE IF NOT EXISTS newestIds ("
             "id TEXT NOT NULL"
             ");");
    db_.exec("CREATE UNIQUE INDEX IF NOT EXISTS \"newestIdsIdx\" ON newestIds (id DESC);");

    //Setup storage
    dataDir_ = QDir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    cacheJsonDir_ = QDir(dataDir_.path() + "/cache_" + objectName() + "/tweets/json");
    cacheMediaDir_ = QDir(dataDir_.path() + "/cache_" + objectName() + "/tweets/media");
    dataDir_.mkpath(cacheJsonDir_.path());
    dataDir_.mkpath(cacheMediaDir_.path());

    //Auth
    auth_ = new O1Twitter(TWITTER_CUSTOMER_KEY, TWITTER_CUSTOMER_SECRET, settings_, this);
    req_  = new SignedRequester(auth_, this);

    //Recover all tweets from cache
    QStringList fileNames = cacheJsonDir_.entryList(QDir::Files);
    foreach (QString fileName, fileNames) {
        QFile file(cacheJsonDir_.filePath(fileName));
        file.open(QFile::ReadOnly);
        Tweet tweet(QJsonDocument::fromJson(qUncompress(file.readAll())).object());
        tweets_.insert(tweet.id(), tweet);
        file.close();
    }

    updateIds();

    traceDebug() << QString::number(tweets_.size()) + " twitts loaded." << flush;

    //Initialize stream listener
    stream_ = new TwitterStream(auth_, this);
    connect(stream_, SIGNAL(newTweets()), this, SLOT(onStreamNewTweets()), Qt::QueuedConnection);

    //Login
    if (auth_->login())
        loginState_ = 1;
    else
        loginState_ = 255;

    int refreshTime = 10000; //Program first refresh on 10 sec.

    //If there are less than 200 tweets, get up to 200 from home timeline
    if (tweets_.size() < 200) {
        QJsonArray arr = homeTimeline(0, 0, 200);
        for (int i = arr.count() - 1; i >= 0; --i) {
            Tweet tweet(arr[i].toObject());
            tweets_.insert(tweet.id(), tweet);
        }
        updateIds();
        refreshTime = 60000; //Program first refresh on 1 min.
    }

    //Setup timer for continuous refreshing from REST
    refreshTimer_->setInterval(refreshTime);

    //Request for Twitter configuration
    QVariant lastConfRequest = settings_->value("lastConfigurationJsonRequest");
    QString day = QString::number(QDateTime::currentMSecsSinceEpoch() / 86400000);
    if (lastConfRequest.isNull() || (day != lastConfRequest.toString())) {
        settings_->setValue("lastConfigurationJsonRequest", day);
        QByteArray configuration = req_->doAuthorizedRequest(shr_, TWITTER_CONFIGURATION_URI);
        settings_->setValue("configurationJson", QString(configuration));
        twitterConfiguration_ = TwitterConfiguration(QJsonDocument::fromJson(configuration).object());
    } else {
        QByteArray configuration = settings_->value("configurationJson").toByteArray();
        twitterConfiguration_ = TwitterConfiguration(QJsonDocument::fromJson(configuration).object());
    }
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

    loginState_ = 0;
    streamConnectionState_ = 0;

    tweets_.clear();

    lastRefreshedTweetId_ = 0;

    if (newest_id_ != 0)
        db_.exec("INSERT OR REPLACE INTO newestIds (id) VALUES ('" + QString::number(newest_id_) + "');");

    oldest_id_ = 0;
    newest_id_ = 0;

    if (db_.isOpen()) {
        db_.commit();
        db_.close();
    }
}

void Twitter::connectToStream(QString uri)
{
    traceDebug();

    if (this->thread() != QThread::currentThread()) {
        traceDebug() << "Warning: This function has been called from diferent thread!" << flush;
    }

    QMutexLocker locker(&mutex_);

    if (stream_->start(uri))
        streamConnectionState_ = 1;
    else
        streamConnectionState_ = 255;
}

void Twitter::setAutoRefresh(bool autoRefresh)
{
    QMutexLocker locker(&mutex_);

    if (autoRefresh && (loginState_ == 1) && (refreshTimer_->interval() != 0)) {
        refreshTimer_->start();
    } else if (!autoRefresh && refreshTimer_->isActive()) {
        refreshTimer_->stop();
    }
}

void Twitter::updateStatus(TwitterStatus status)
{
    QMutexLocker locker(&mutex_);

    req_->doAuthorizedRequest(shr_, TWITTER_UPDATE_STATUS_URI, status.params(), "POST");
}

void Twitter::onStreamNewTweets()
{
    if (!mutex_.tryLock(100))
        return;

    traceDebug();

    TweetsMap tweets = stream_->getTweets();

    foreach (Tweet tweet, tweets) {
        tweets_.insert(tweet.id(), tweet);
    }

    updateIds();

    //Store all tweets to db
    updateTweetsCache(tweets);

    traceDebug() << tweets.count() << " new tweets " << currentPage_;

    if (currentPage_ == 0)
        emit newTweets(tweets);

    mutex_.unlock();
}

void Twitter::onRefreshTimerTimeout()
{
    QMutexLocker locker(&mutex_);

    if (loginState_ != 1)
        return;

    if (tweets_.empty())
        return;

    updateTweetsData();

    recoverOfflineTweets();

    refreshTimer_->setInterval(60000); //1 min.
}

const TwitterConfiguration& Twitter::twitterConfiguration() const
{
    return twitterConfiguration_;
}

bool Twitter::autoRefresh()
{
    QMutexLocker locker(&mutex_);
    return refreshTimer_->isActive();
}

unsigned char Twitter::streamConnectionState()
{
    QMutexLocker locker(&mutex_);
    return streamConnectionState_;
}

int Twitter::tweetsCount()
{
    QMutexLocker locker(&mutex_);
    return tweets_.size();
}

unsigned char Twitter::loginState()
{
    QMutexLocker locker(&mutex_);
    return loginState_;
}

QJsonArray Twitter::homeTimeline(qint64 max_id, qint64 since_id, int count)
{
    QMap<QString, QString> params;
    if (max_id > 0) {
        params.insert("max_id", QString::number(max_id));
    }
    if (since_id > 0) {
        params.insert("since_id", QString::number(since_id));
    }
    params.insert("count", QString::number(count));

    QJsonArray jsonTweets = QJsonDocument::fromJson(req_->doAuthorizedRequest(shr_, TWITTER_HOME_TIMELINE_URI, params)).array();

    return jsonTweets;
}

void Twitter::updateTweetsCache(const TweetsMap &tweets)
{
    if (tweets.size() == 0)
        return;

    foreach (Tweet tweet, tweets) {
        QFile file(cacheJsonDir_.filePath(tweet.idStr()+".json.gz"));
        file.open(QFile::WriteOnly | QFile::Truncate);
        file.write(qCompress(QJsonDocument(tweet.jsonObj()).toJson()));
        file.close();
    }
}

void Twitter::updateIds()
{
    if (tweets_.empty()) {
        newest_id_ = 0;
        oldest_id_ = 0;
    } else {
        newest_id_ = tweets_.last().id();
        oldest_id_ = tweets_.first().id();
    }
}

void Twitter::updateTweetsData()
{
    traceDebug();

    if (lastRefreshedTweetId_ == 0) {
        if (tweets_.size() == 0) {
            traceDebug() << "Nothing to update";
            return;
        }

        lastRefreshedTweetId_ = tweets_.first().id();
    }

    //Get up to 6 x 100 tweets to update status
    TweetsMap udatedTweets;
    for (int i = 0; (i < 6) && (lastRefreshedTweetId_ != tweets_.last().id()); ++i) {
        //Get up to 100 tweets to update status
        QStringList ids;
        TweetsMap::iterator it = tweets_.find(lastRefreshedTweetId_);
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
            udatedTweets.insert(tweet.id(), tweet);
            tweets_.insert(tweet.id(), tweet);
        }
    }

    if (lastRefreshedTweetId_ == tweets_.last().id())
        lastRefreshedTweetId_ = tweets_.first().id();

    if (!udatedTweets.empty()) {
        updateTweetsCache(udatedTweets);
        if (pageTweets_.contains(udatedTweets.first().id()) || (pageSize_ == 0))
            emit newTweets(udatedTweets);
    }

    traceDebug() << udatedTweets.count() << " tweets updated";
}

void Twitter::recoverOfflineTweets()
{
    traceDebug();

    TweetsMap tweets;
    qint64 newestId = 0;
    //Recover all tweets from cache
    QSqlQuery query = db_.exec("SELECT * FROM newestIds;");
    if (query.next()) {
        newestId = GetField(query, "id").toString().toLongLong();
    } else {
        traceDebug() << "Nothing to recover";
        return;
    }
    //Get up to 200 tweets from home timeline
    QJsonArray arr = homeTimeline(0, newestId);
    for (int i = arr.count() - 1; i >= 0; --i){
        Tweet tweet(arr[i].toObject());
        if (tweets_.contains(tweet.id())) {
            db_.exec("DELETE FROM newestIds WHERE (id='" + QString::number(newestId) + "');");
            break;
        } else {
            tweets.insert(tweet.id(), tweet);
            tweets_.insert(tweet.id(), tweet);
            if (pageSize_)
                pageTweets_.insert(tweet.id(), tweet);
        }
    }

    updateIds();

    if (!tweets.empty()) {
        updateTweetsCache(tweets);
        emit newOfflineTweets(tweets);
    }

    traceDebug() << tweets.count() << " new offline tweets loaded";
}
