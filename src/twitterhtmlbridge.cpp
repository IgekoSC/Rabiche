#include "twitterhtmlbridge.h"
#include <QDebug>

#define traceDebug() qDebug() << QThread::currentThreadId() << __PRETTY_FUNCTION__

TwitterHtmlBridge::TwitterHtmlBridge(Twitter *twitter, QWebPage *webPage, QObject *parent) : QObject(parent)
{
    firstTime_ = true;
    page_ = 0;
    pageSize_ = 100;

    twitter_ = twitter;

    webPage_ = webPage;
    webPage_->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    webPage_->mainFrame()->setZoomFactor(1.0);

    connect(webPage_, SIGNAL(linkClicked(QUrl)), this, SLOT(onLinkClicked(QUrl)));
    connect(webPage_, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished(bool)));
    connect(webPage_->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(onJavaScriptWindowObjectCleared()));
    connect(twitter_, SIGNAL(newTweets(TweetsMap)), this, SLOT(onNewTweets(TweetsMap)));
    connect(twitter_, SIGNAL(newOfflineTweets(TweetsMap)), this, SLOT(onNewOfflineTweets(TweetsMap)));

    init();
}

TwitterHtmlBridge::~TwitterHtmlBridge()
{

}

QMap<QString, QVariant> TwitterHtmlBridge::newHtmlTweets()
{
    traceDebug();

    QMap<QString, QVariant> tmp = newHtmlTweets_;

    newHtmlTweets_.clear();

    return tmp;
}

QMap<QString, QVariant> TwitterHtmlBridge::newOfflineHtmlTweets()
{
    QMap<QString, QVariant> tmp = newOfflineHtmlTweets_;

    newOfflineHtmlTweets_.clear();

    return tmp;
}

void TwitterHtmlBridge::tweet(QString text)
{
    traceDebug();
}

void TwitterHtmlBridge::onLinkClicked(const QUrl &url)
{
    qDebug() << url;

    if (url.toString().startsWith("http")) {
        QDesktopServices::openUrl(url);
    } else if (url.toString().startsWith("goto://?page=")) {
        goToPage(url.toString().mid(13).toInt());
    }
}

void TwitterHtmlBridge::loadFinished(bool b)
{
    traceDebug();

    if (firstTime_) {
        firstTime_ = false;
    }

    webPage_->mainFrame()->evaluateJavaScript("init();");
}

void TwitterHtmlBridge::onJavaScriptWindowObjectCleared()
{
    traceDebug();

    webPage_->mainFrame()->addToJavaScriptWindowObject("twitter", this);
}

void TwitterHtmlBridge::onNewTweets(TweetsMap tweets)
{
    QMap<QString, QVariant> htmlTweets = twittsToHtml(tweets);
    QStringList ids = htmlTweets.keys();
    foreach (QString id, ids) {
        newHtmlTweets_.insert(id, htmlTweets[id].toString());
    }

    traceDebug() << newHtmlTweets_.size() << "==" << tweets.size();

    webPage_->mainFrame()->evaluateJavaScript("onNewTweets();");
}

void TwitterHtmlBridge::onNewOfflineTweets(TweetsMap tweets)
{
    QMap<QString, QVariant> htmlTweets = twittsToHtml(tweets);
    QStringList ids = htmlTweets.keys();
    foreach (QString id, ids) {
        newOfflineHtmlTweets_.insert(id, htmlTweets[id].toString());
    }

    webPage_->mainFrame()->evaluateJavaScript("onNewOfflineTweets();");
}

void TwitterHtmlBridge::init()
{
    QString html;
    QTextStream ss(&html);

    ss <<
    "<html>"
    "   <head>"
    "      <link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.1/css/bootstrap.min.css\">"
    "      <link rel=\"stylesheet\" href=\"qrc:/Resources/webui/style.css\">"
    "      <script src=\"https://ajax.googleapis.com/ajax/libs/jquery/2.1.3/jquery.min.js\"></script>"
    "      <script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.1/js/bootstrap.min.js\"></script>"
    "      <script src=\"qrc:/Resources/webui/jquery.lazyload.min.js\"></script>"
    "      <script src=\"qrc:/Resources/webui/main.js\"></script>"
    "   </head>"
    "   <body>"
    "      <div class=\"container\">";
    QMap<QString, QVariant> tweets = twittsToHtml(twitter_->tweets(page_, pageSize_));
    QString htmlTweets;
    foreach (QVariant tweet, tweets) {
        htmlTweets = tweet.toString() + htmlTweets;
    }
    QString pagination = htmlPagination();
    ss << pagination << htmlTweets << pagination <<
    "      </div>"
    "   </body>"
    "</html>";

    webPage_->mainFrame()->setHtml(html);
}



QMap<QString, QVariant> TwitterHtmlBridge::twittsToHtml(TweetsMap tweets)
{
    QMap<QString, QVariant> tweetsHtml;
    QString tweetHtml;
    QTextStream ss(&tweetHtml);

    foreach (Tweet tweet, tweets) {
        ss <<
        "<div class=\"row\" id=\"" << tweet.idStr() << "\">"
        "   <div class=\"twitt col-xs-12 col-sm-12\">"
        "      <div class=\"media\">"
        "         <a class=\"media-left\" href=\"user://" << tweet.user().screenName() << "\">"
        "            <img src=\"" << tweet.user().profileImageUrl() << "\" alt=\"Avatar\">"
        "         </a>"
        "         <div class=\"media-body\">"
        "            <p class=\"media-heading\">"
        "               <span class=\"fullname\">" << tweet.user().name() << "</span>"
        "               <span class=\"username\">@" << tweet.user().screenName() << "</span>"
        "            <p>"
        "            <p>" << tweet.htmlText() << "</p>";
        foreach (TwitterMedia media, tweet.entities().media()) {
            ss <<
            "        <div class=\"image\">"
            "           <img data-original=\"" << media.mediaUrl() << ":large\""
            "           style=\"width:" << media.sizeLarge().w << "px; height:" << media.sizeLarge().h << "px;\""
            "           src=\"qrc:/Resources/img/blank.png\" class=\"lazy\">"
            "        </div>";
        }
        ss <<
        "            <div class=\"reply\">"
        "               <a href=\"reply://" << tweet.idStr() << "\">"
        "                  <div class=\"icon\"></div>"
        "               </a>"
        "               <a href=\"replies://" << tweet.idStr() << "\">" << 0 << "</a>"
        "            </div>"
        "            <div class=\"retweet\">"
        "               <a href=\"retweet://" << tweet.idStr() << "\">"
        "                  <div class=\"icon\"></div>"
        "               </a>"
        "               <a href=\"retweets://" << tweet.idStr() << "\">" << tweet.retweetCount() << "</a>"
        "            </div>"
        "            <div class=\"favorite\">"
        "               <a href=\"favorite://" << tweet.idStr() << "\">"
        "                  <div class=\"icon\"></div>"
        "               </a>"
        "               <a href=\"favorites://" << tweet.idStr() << "\">" << tweet.favoriteCount() << "</a>"
        "            </div>"
        "         </div>"
        "      </div>"
        "   </div>"
        "</div>";

        tweetsHtml.insert(tweet.idStr(), tweetHtml);
        tweetHtml.clear();
    }

    return tweetsHtml;
}

QString TwitterHtmlBridge::htmlPagination()
{
    QString html;
    QTextStream ss(&html);

    int nPages = twitter_->tweetsCount() / pageSize_;

    int first = page_ - 5;
    if (first < 0)
        first = 0;

    ss <<
    "<nav>"
    "   <ul class=\"pagination\">";
    if (page_ > 0) {
        ss <<
        "  <li>"
        "     <a href=\"goto://?page=" << page_-1 << "\" aria-label=\"First\">"
        "        <span aria-hidden=\"true\">&laquo;</span>"
        "     </a>"
        "  </li>";
    }
    int page = first;
    for (int i = 0; i < 10; ++i) {
        page = first+i;
        if (page == page_)
            ss << "<li class=\"active\"><a href=\"goto://?page=" << page << "\">" << page << "</a></li>";
        else
            ss << "<li><a href=\"goto://?page=" << page << "\">" << page << "</a></li>";
        if (page >= nPages)
            break;
    }
    if (page_ < nPages) {
        ss <<
        "  <li>"
        "     <a href=\"goto://?page=" << page_+1 << "\" aria-label=\"Last\">"
        "        <span aria-hidden=\"true\">&raquo;</span>"
        "     </a>"
        "  </li>";
    }
    ss <<
    "   </ul>"
    "</nav>";

    return html;
}

void TwitterHtmlBridge::goToPage(int page)
{
    page_ = page;

    init();
}
