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
    Q_INVOKABLE QJsonArray tweets();
    Q_INVOKABLE const QJsonArray& allTweets();

signals:
    void newTweets();
    void tweetDeleted();

private slots:
    void onNewTweets(TweetsMap tweet);

private:
    Twitter* twitter_;
    QThread* thread_;
    QJsonArray allTweets_;
    QJsonArray newTweets_;

};

#endif // RABICHE_H
