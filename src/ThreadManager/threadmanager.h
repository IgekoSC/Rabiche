#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <QObject>
#include <QHash>
#include <QList>
#include <QQueue>
#include <QThread>
#include <QRunnable>
#include <QMutexLocker>
#include <QThreadPool>
#include <ThreadManager/worker.h>

class ThreadManager : public QObject
{
    Q_OBJECT
public:
    explicit ThreadManager(QObject *parent = 0);
    bool addWorker(Worker* worker);
    bool removeWorker(QString name, bool waitUntilFinish = false);
    int getMaxRunningThreads();
    void setMaxRunningThreads(int value);

signals:
    void workerFinished(Worker* worker);

public slots:
    bool startWorker(QString name);

private slots:
    void onWorkerFinished(QThread *thread);

private:
    QList<QThread*> freeThreads_;
    QList<QThread*> bussyThreads_;
    QHash<QString, Worker*> workers_;
    QQueue<Worker*> workersQueue_;
    QMutex mutex_;
    int maxRunningThreads_;



};

#endif // THREADMANAGER_H
