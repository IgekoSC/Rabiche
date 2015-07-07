#include "threadmanager.h"

#include <QDebug>
#define traceDebug() qDebug() << QThread::currentThreadId() << __PRETTY_FUNCTION__

ThreadManager::ThreadManager(QObject *parent) : QObject(parent)
{
    maxRunningThreads_ = 5;

    QThread* thread = new QThread(this);
    freeThreads_.push_back(thread);
    thread->start();
}

bool ThreadManager::addWorker(Worker *worker)
{
    QMutexLocker locker(&mutex_);

    if (workers_.contains(worker->objectName()))
        return false;

    workers_.insert(worker->objectName(), worker);
    connect(worker, SIGNAL(finished(QThread*)), this, SLOT(onWorkerFinished(QThread*)));

    return true;
}

bool ThreadManager::removeWorker(QString name, bool waitUntilFinish)
{
    QMutexLocker locker(&mutex_);

    if (!workers_.contains(name))
        return false;

    Worker* worker = workers_[name];

    while (waitUntilFinish && worker->working())
        QThread::msleep(10);

    if (worker->working())
        return false;

    disconnect(worker, SIGNAL(finished(QThread*)), this, SLOT(onWorkerFinished(QThread*)));
    workers_.remove(name);
    delete worker;

    return true;
}

bool ThreadManager::startWorker(QString name)
{
    QMutexLocker locker(&mutex_);

    if (!workers_.contains(name))
        return false;

    Worker* worker = workers_[name];

    QThread* thread;
    if (freeThreads_.size() > 0) {
        thread = freeThreads_.front();
        freeThreads_.pop_front();
        bussyThreads_.push_back(thread);
    } else if (bussyThreads_.size() < maxRunningThreads_) {
        thread = new QThread(this);
        bussyThreads_.push_back(thread);
        thread->start();
        while (!thread->isRunning())
            QThread::msleep(10);
    } else {
        workersQueue_.push_back(worker);
        return true;
    }

    worker->moveToThread(thread);
    QMetaObject::invokeMethod(worker, "doWork");

    return true;
}

void ThreadManager::onWorkerFinished(QThread* thread)
{
    QMutexLocker locker(&mutex_);

    Worker* worker = (Worker*)QObject::sender();

    while (worker->thread() != this->thread())
        QThread::msleep(10);

    if (workersQueue_.size() > 0) {
        Worker* queueWorker = workersQueue_.front();
        workersQueue_.pop_front();
        queueWorker->moveToThread(thread);
        QMetaObject::invokeMethod(queueWorker, "doWork");
    } else {
        bussyThreads_.removeOne(thread);
        if (freeThreads_.size() < (maxRunningThreads_ / 2)) {
            freeThreads_.push_back(thread);
        } else {
            thread->quit();
            while (thread->isRunning())
                QThread::sleep(10);
            delete thread;
        }
    }

    emit workerFinished(worker);
}

int ThreadManager::getMaxRunningThreads()
{
    QMutexLocker locker(&mutex_);
    return maxRunningThreads_;
}

void ThreadManager::setMaxRunningThreads(int value)
{
    QMutexLocker locker(&mutex_);
    maxRunningThreads_ = value;
}




