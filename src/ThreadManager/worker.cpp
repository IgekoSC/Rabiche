#include "worker.h"
#include <QUuid>

#include <QDebug>
#define traceDebug() qDebug() << QThread::currentThreadId() << __PRETTY_FUNCTION__

Worker::Worker(QString name, QObject* parent)
    : QObject(parent)
{
    {
        QMutexLocker locker(&workingMutex_);
        working_ = false;
    }
    lastMsecsSinceEpoch_ = 0;
    interval_ = 5000;
    if (name == "")
        name = QUuid::createUuid().toString();
    setObjectName(name);
    {
        QMutexLocker locker(&firstTimeMutex_);
        firstTime_ = true;
    }
}

Worker::~Worker()
{
    traceDebug() << objectName() + "::~Worker()" << flush;
    QMutexLocker locker_1(&mutex_);
    QMutexLocker locker_2(&workingMutex_);
    QMutexLocker locker_3(&intervalMutex_);
    QMutexLocker locker_5(&firstTimeMutex_);
}

bool Worker::working()
{
    QMutexLocker locker(&workingMutex_);
    return working_;
}

int Worker::interval()
{
    QMutexLocker locker(&intervalMutex_);
    return interval_;
}

void Worker::setInterval(const int &interval)
{
    QMutexLocker locker(&intervalMutex_);
    interval_ = interval;
}

bool Worker::timeOut()
{
    if (working())
        return false;

    QMutexLocker locker(&intervalMutex_);
    return ((lastMsecsSinceEpoch_ + interval_) <= QDateTime::currentMSecsSinceEpoch());
}

bool Worker::firstTime()
{
    QMutexLocker locker(&firstTimeMutex_);
    return firstTime_;
}

void Worker::doWork()
{
    {
        QMutexLocker locker(&workingMutex_);
        working_ = true;
    }

    if (firstTime())
        beforeFirstWork();
    beforeWork();

    //Do work
    work();

    if (firstTime())
        afterFirstWork();
    afterWork();

    //Store current date in seconds
    lastMsecsSinceEpoch_ = QDateTime::currentMSecsSinceEpoch();

    {
        QMutexLocker locker(&firstTimeMutex_);
        firstTime_ = false;
    }

    {
        QMutexLocker locker(&workingMutex_);
        working_ = false;
    }

    emit finished(thread());

    moveToThread(QObject::sender()->thread());
}
