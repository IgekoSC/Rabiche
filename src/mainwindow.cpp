#include "mainwindow.h"

#define traceDebug() qDebug() << QThread::currentThreadId() << __PRETTY_FUNCTION__

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    //Settings
    ConecToDb(db_, "rabiche_settings");
    settings_ = new SqlSettings(&db_, this);
    settings_->setSecure(false);

    //Setup default settings
    if (settings_->value("new_tweet_dock_location").isNull()) {
        settings_->setValue("new_tweet_dock_location", (int)Qt::RightDockWidgetArea);
    }
    if (settings_->value("new_tweet_dock_visivility").isNull()) {
        settings_->setValue("new_tweet_dock_visivility", true);
    }

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
    QMetaObject::invokeMethod(twitter_, "setAutoRefresh", Q_ARG(bool, false));
    //QMetaObject::invokeMethod(twitter_, "connectToStream", Q_ARG(QString, TWITTER_USER_STREAM_URI));

    webView_ = new QWebView(this);
    webView_->setMinimumSize(800, 600);

    htmlBridge_ = new TwitterHtmlBridge(twitter_, webView_->page(), this);

    //GUI
    setCentralWidget(webView_);

    newTweetDock_ = new QDockWidget(tr("New tweet"), this);
    wgtNewTweet_ = new WgtNewTweet(twitter_->twitterConfiguration(), newTweetDock_);
    newTweetDock_->setWidget(wgtNewTweet_);
    this->addDockWidget((Qt::DockWidgetArea)settings_->value("new_tweet_dock_location").toInt(), newTweetDock_);
    newTweetDock_->setHidden(settings_->value("new_tweet_dock_visivility").toBool());

    //Menu bar
    mnuWindows_ = new QMenu(tr("Windows"), this);
    menuBar()->addMenu(mnuWindows_);

    //Actions
    actShowNewTweet_ = newTweetDock_->toggleViewAction();
    mnuWindows_->addAction(actShowNewTweet_);

    //Connections
    connect(wgtNewTweet_, SIGNAL(updateStatus(TwitterStatus)), twitter_, SLOT(updateStatus(TwitterStatus)));
}

MainWindow::~MainWindow()
{
    twitter_->loguout();

    thread_->quit();
    while (thread_->isRunning())
        QThread::msleep(1);

    twitter_->deleteLater();

    settings_->setValue("new_tweet_dock_location", (int)dockWidgetArea(newTweetDock_));
    settings_->setValue("new_tweet_dock_visivility", newTweetDock_->isHidden());
}
