#include "mainwindow.h"

#define traceDebug() qDebug() << QThread::currentThreadId() << __PRETTY_FUNCTION__

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowIcon(QIcon(":/Resources/img/icon.png"));
    setWindowIconText("Rabiche");

    thread_ = new QThread(this);
    thread_->start();
    while (!thread_->isRunning())
        QThread::msleep(1);

    twitter_ = new Twitter("eliasrm87");
    twitter_->moveToThread(thread_);
    QMetaObject::invokeMethod(twitter_, "login");
    while (twitter_->loginState() == 0)
        QThread::msleep(1);
    if (twitter_->loginState() != 1) {
        qDebug() << "Login fail: " << twitter_->loginState() << flush;
        return;
    }
    //    QMetaObject::invokeMethod(twitter_, "connectToStream", Q_ARG(QString, TWITTER_USER_STREAM_URI));

    webView_ = new QWebView(this);

    htmlBridge_ = new TwitterHtmlBridge(twitter_, webView_->page(), this);

    setCentralWidget(webView_);
}

MainWindow::~MainWindow()
{
    twitter_->loguout();

    thread_->quit();
    while (thread_->isRunning())
        QThread::msleep(1);

    twitter_->deleteLater();
}
