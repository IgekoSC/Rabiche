#ifndef RABICHE_H
#define RABICHE_H

#include <QObject>
#include <QThread>
#include <twitter.h>

class Rabiche : public QObject
{
    Q_OBJECT
public:
    explicit Rabiche(QObject *parent = 0);
    ~Rabiche();
    Q_INVOKABLE void end();
    Q_INVOKABLE QJsonArray getNewTweets();
    Q_INVOKABLE QJsonArray getNewOfflineTweets();
    Q_INVOKABLE void openUrl(QString url);

signals:
    void newTweets();
    void deletedTweets();
    void newOfflineTweets();

private slots:
    void onNewTweets(TweetsMap tweet);
    void onNewOfflineTweets(TweetsMap tweets);

private:
    Twitter* twitter_;
    QThread* thread_;
    QJsonArray newTweets_;
    QJsonArray offlineTweets_;
    QJsonArray deletedTweets_;

};

#endif // RABICHE_H
