#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QDateTime>
#include <QMutexLocker>
#include <QRunnable>

class Worker : public QObject
{
    Q_OBJECT
public:
    Worker(QString name = "", QObject *parent = 0);
    ~Worker();
    bool working();
    int interval();
    void setInterval(const int& interval);
    /** Return true if it's time to do work based on
     *  last work time and the interval.
    **/
    bool timeOut();

public slots:
    void doWork();

signals:
    void finished(QThread* thread);

protected:
    virtual void work() = 0;
    bool firstTime();

    //Functions that can be reimplemented optionally
    ///Function called before first time work is executed
    virtual void beforeFirstWork() {}
    ///Function called after first time work is executed
    virtual void afterFirstWork() {}
    ///Function called before work is executed
    virtual void beforeWork() {}
    ///Function called after work is executed
    virtual void afterWork() {}

    //Atributes
    ///Mutex to be used by all public functions declared by all child classes
    QMutex mutex_;

private:
    bool working_;
    QMutex workingMutex_;
    qint64 interval_;
    QMutex intervalMutex_;
    qint64 lastMsecsSinceEpoch_;
    bool firstTime_;
    QMutex firstTimeMutex_;

};

#endif // WORKER_H
