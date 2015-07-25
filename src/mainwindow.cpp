#include "mainwindow.h"
#include <QWebFrame>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    firstTime_ = true;

    thread_ = new QThread(this);
    thread_->start();
    while (!thread_->isRunning())
        QThread::msleep(1);

    twitter_ = new Twitter("eliasrm87");
    twitter_->moveToThread(thread_);

    webView_ = new QWebView(this);
    webView_->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    webView_->page()->mainFrame()->addToJavaScriptWindowObject("twitter", this);
    setCentralWidget(webView_);

    connect(webView_, SIGNAL(linkClicked(QUrl)), this, SLOT(onLinkClicked(QUrl)));
    connect(webView_, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished(bool)));
    connect(webView_->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(onJavaScriptWindowObjectCleared()));
    connect(twitter_, SIGNAL(newTweets(TweetsMap)), this, SLOT(onNewTweets(TweetsMap)));
    connect(twitter_, SIGNAL(newOfflineTweets(TweetsMap)), this, SLOT(onNewOfflineTweets(TweetsMap)));

    QMetaObject::invokeMethod(twitter_, "login");

    while (twitter_->loginState() == 0)
        QThread::msleep(1);

    if (twitter_->loginState() != 1) {
        qDebug() << "Login fail: " << twitter_->loginState() << flush;
        return;
    }

    QMetaObject::invokeMethod(twitter_, "connectToStream", Q_ARG(QString, TWITTER_USER_STREAM_URI));

    init();
}

MainWindow::~MainWindow()
{
    twitter_->loguout();

    thread_->quit();
    while (thread_->isRunning())
        QThread::msleep(1);

    twitter_->deleteLater();
}

QMap<QString, QVariant> MainWindow::newHtmlTweets()
{
    QMap<QString, QVariant> tmp = newHtmlTweets_;

    newHtmlTweets_.clear();

    return tmp;
}

QMap<QString, QVariant> MainWindow::newOfflineHtmlTweets()
{
    QMap<QString, QVariant> tmp = newOfflineHtmlTweets_;

    newOfflineHtmlTweets_.clear();

    return tmp;
}

void MainWindow::init()
{
    QString html;
    QTextStream ss(&html);

    ss <<
    "      <html>"
    "         <head>"
    "            <link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.1/css/bootstrap.min.css\">"
    "            <link rel=\"stylesheet\" href=\"qrc:/Resources/webui/style.css\">"
    "            <script src=\"https://ajax.googleapis.com/ajax/libs/jquery/2.1.3/jquery.min.js\"></script>"
    "            <script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.1/js/bootstrap.min.js\"></script>"
    "            <script src=\"qrc:/Resources/webui/jquery.lazyload.min.js\"></script>"
    "            <script src=\"qrc:/Resources/webui/main.js\"></script>"
    "         </head>"
    "         <body onload=\"init()\">"
    "            <div class=\"container\">";
    QMap<QString, QVariant> tweets = twittsToHtml(twitter_->tweets());
    QString htmlTweets;
    foreach (QVariant tweet, tweets) {
        htmlTweets = tweet.toString() + htmlTweets;
    }
    ss << htmlTweets <<
    "            </div>"
    "         </body>"
    "      </html>";

    webView_->setHtml(html);
}

void MainWindow::onLinkClicked(const QUrl &url)
{
    qDebug() << url;
}

void MainWindow::loadFinished(bool b)
{
    if (firstTime_) {
        firstTime_ = false;

    }
}

void MainWindow::onJavaScriptWindowObjectCleared()
{
    webView_->page()->mainFrame()->addToJavaScriptWindowObject("twitter", this);
}

void MainWindow::onNewTweets(TweetsMap tweets)
{
    QMap<QString, QVariant> htmlTweets = twittsToHtml(tweets);
    QStringList ids = htmlTweets.keys();
    foreach (QString id, ids) {
        newHtmlTweets_.insert(id, htmlTweets[id].toString());
    }

    emit newTweets();
}

void MainWindow::onNewOfflineTweets(TweetsMap tweets)
{
    QMap<QString, QVariant> htmlTweets = twittsToHtml(tweets);
    QStringList ids = htmlTweets.keys();
    foreach (QString id, ids) {
        newOfflineHtmlTweets_.insert(id, htmlTweets[id].toString());
    }

    emit newOfflineTweets();
}

QMap<QString, QVariant> MainWindow::twittsToHtml(TweetsMap tweets)
{
    QMap<QString, QVariant> tweetsHtml;
    QString tweetHtml;
    QTextStream ss(&tweetHtml);

    foreach (Tweet tweet, tweets) {
        ss <<
        "      <div class=\"row\" id=\"" << tweet.idStr() << "\">"
        "         <div class=\"twitt col-xs-12 col-sm-12\">"
        "            <div class=\"media\">"
        "               <a class=\"media-left\" href=\"user://" << tweet.user().screenName() << "\">"
        "                  <img src=\"" << tweet.user().profileImageUrl() << "\" alt=\"Avatar\">"
        "               </a>"
        "               <div class=\"media-body\">"
        "                  <p class=\"media-heading\">"
        "                     <span class=\"fullname\">" << tweet.user().name() << "</span>"
        "                     <span class=\"username\">@" << tweet.user().screenName() << "</span>"
        "                  <p>"
        "                  <p>" << tweet.htmlText() << "</p>";
        foreach (TwitterMedia media, tweet.entities().media()) {
            ss <<
            "              <div class=\"image\">"
            "                 <img data-original=\"" << media.mediaUrl() << ":large\""
            "                 src=\"qrc:/Resources/img/blank.png\" class=\"img-responsive lazy\">"
            "              </div>";
        }
        ss <<
        "                  <div class=\"reply\">"
        "                     <a href=\"reply://" << tweet.idStr() << "\">"
        "                        <div class=\"icon\"></div>"
        "                     </a>"
        "                     <a href=\"replies://" << tweet.idStr() << "\">" << 0 << "</a>"
        "                  </div>"
        "                  <div class=\"retweet\">"
        "                     <a href=\"retweet://" << tweet.idStr() << "\">"
        "                        <div class=\"icon\"></div>"
        "                     </a>"
        "                     <a href=\"retweets://" << tweet.idStr() << "\">" << tweet.retweetCount() << "</a>"
        "                  </div>"
        "                  <div class=\"favorite\">"
        "                     <a href=\"favorite://" << tweet.idStr() << "\">"
        "                        <div class=\"icon\"></div>"
        "                     </a>"
        "                     <a href=\"favorites://" << tweet.idStr() << "\">" << tweet.favoriteCount() << "</a>"
        "                  </div>"
        "               </div>"
        "            </div>"
        "         </div>"
        "      </div>";

        tweetsHtml.insert(tweet.idStr(), tweetHtml);
        tweetHtml.clear();
    }

    return tweetsHtml;
}
