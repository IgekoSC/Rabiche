#include "o1.h"
#include <QMessageAuthenticationCode>

#define TIMERINTERVAL 300000 //5 min
#define PORT 8888

O1::O1(const QString &requestTokenURL, const QString &authorizeURL, const QString &accessTokenURL, const QString &consumerKey, const QString &consumerSecret, SqlSettings *settings, QObject *parent) :
    QObject(parent), requestTokenURL_(requestTokenURL), authorizeURL_(authorizeURL), accessTokenURL_(accessTokenURL), consumerKey_(consumerKey), consumerSecret_(consumerSecret), settings_(settings)
{
    oAuthTokenSecret_ = "";
    shr_ = new SynchronousHttpRequest(this);
    initServer();

    oAuthToken_       = settings_->value("oAuthToken").toString();
    oAuthVerifier_    = settings_->value("oAuthVerifier").toString();
    oAuthTokenSecret_ = settings_->value("oAuthTokenSecret").toString();
}

O1::~O1()
{

}

bool O1::logedIn() {
    return !oAuthToken_.isEmpty();
}

bool O1::login()
{
    if (!logedIn()) {
        requestToken();
        authorize();
        accessToken();
    }

    return true;
}

QString O1::requestTokenURL() const
{
    return requestTokenURL_;
}

void O1::setRequestTokenURL(const QString &requestTokenURL)
{
    requestTokenURL_ = requestTokenURL;
}
QString O1::authorizeURL() const
{
    return authorizeURL_;
}

void O1::setAuthorizeURL(const QString &authorizeURL)
{
    authorizeURL_ = authorizeURL;
}
QString O1::accessTokenURL() const
{
    return accessTokenURL_;
}

void O1::setAccessTokenURL(const QString &accessTokenURL)
{
    accessTokenURL_ = accessTokenURL;
}
QString O1::consumerKey() const
{
    return consumerKey_;
}

void O1::setConsumerKey(const QString &consumerKey)
{
    consumerKey_ = consumerKey;
}
QString O1::consumerSecret() const
{
    return consumerSecret_;
}

void O1::setConsumerSecret(const QString &consumerSecret)
{
    consumerSecret_ = consumerSecret;
}

QString O1::getOAuthToken() const
{
    return oAuthToken_;
}

QString O1::getOAuthVerifier() const
{
    return oAuthVerifier_;
}

QString O1::getConsumerKey() const
{
    return consumerKey_;
}

QString O1::sign(QString httpMethod, QString url, QMap<QString, QString> params)
{
    //Construct parameters string
    QStringList keys = params.keys();

    QStringList encodedKeys;
    foreach (QString key, keys) {
        encodedKeys.push_back(QUrl::toPercentEncoding(key));
    }

    encodedKeys.sort();
    QByteArray parametersString = "";
    for (int i = 0; i < encodedKeys.size(); ++i) {
        QByteArray encodedKey = encodedKeys[i].toUtf8();
        QByteArray key = QByteArray::fromPercentEncoding(encodedKey);
        parametersString += encodedKey
                + "="
                + QUrl::toPercentEncoding(params[key]);
        if (i < (encodedKeys.size()-1))
            parametersString += "&";
    }

    //Construct signature base string
    QByteArray signatureBaseString = httpMethod.toUpper().toUtf8()
            + "&"
            + QUrl::toPercentEncoding(url)
            + "&"
            + QUrl::toPercentEncoding(parametersString);

    //Sign
    QByteArray signingKey = QUrl::toPercentEncoding(consumerSecret_)
            + "&"
            + QUrl::toPercentEncoding(oAuthTokenSecret_);

    return QMessageAuthenticationCode::hash(signatureBaseString, signingKey, QCryptographicHash::Sha1).toBase64();
}

QByteArray O1::buildAuthorizationHeader(QMap<QString, QString> headerParameters)
{
    QString authorization = "OAuth ";
    QStringList keys = headerParameters.keys();
    for (int i = 0; i < headerParameters.size(); ++i) {
        QString key = keys[i];
        authorization += key
                + "=\""
                + QUrl::toPercentEncoding(headerParameters[key])
                + "\"";
        if (i < (headerParameters.size()-1))
            authorization += ",";
    }

    return authorization.toUtf8();
}

QMap<QString, QString> O1::parseResponse(const QByteArray &response) {
    QMap<QString, QString> ret;
    foreach (QByteArray param, response.split('&')) {
        QList<QByteArray> kv = param.split('=');
        if (kv.length() == 2) {
            ret.insert(QUrl::fromPercentEncoding(kv[0]), QUrl::fromPercentEncoding(kv[1]));
        }
    }
    return ret;
}

QByteArray O1::nonce() {
    static bool firstTime = true;
    if (firstTime) {
        firstTime = false;
        qsrand(QTime::currentTime().msec());
    }
    QString u = QString::number(QDateTime::currentDateTimeUtc().toTime_t());
    u.append(QString::number(qrand()));
    return u.toUtf8();
}

void O1::requestToken()
{
    QMap<QString, QString> headerParameters;
    headerParameters.insert("oauth_callback", "http://127.0.0.1:8888");
    headerParameters.insert("oauth_consumer_key", consumerKey_);
    headerParameters.insert("oauth_nonce", nonce());
    headerParameters.insert("oauth_timestamp", QString::number(QDateTime::currentDateTimeUtc().toTime_t()));
    headerParameters.insert("oauth_version", "1.0");
    headerParameters.insert("oauth_signature_method", "HMAC-SHA1");
    headerParameters.insert("oauth_signature", sign("POST", requestTokenURL_, headerParameters));

    shr_->reset();
    shr_->setRawHeader("Authorization", buildAuthorizationHeader(headerParameters));

    QMap<QString, QString> response = parseResponse(shr_->post(requestTokenURL_, QByteArray()));

    oAuthToken_ = response.value("oauth_token");
    oAuthTokenSecret_ = response.value("oauth_token_secret");
}

void O1::authorize()
{
    QUrlQuery authorizeQuery;
    authorizeQuery = QUrlQuery();
    authorizeQuery.addQueryItem(QString("oauth_token"), oAuthToken_);

    QUrl url(authorizeURL_);
    url.setQuery(authorizeQuery);

    QDesktopServices::openUrl(QUrl(url));

    QMap<QString, QString> response = startServer();

    oAuthToken_ = response.value("oauth_token");
    oAuthVerifier_ = response.value("oauth_verifier");
}

void O1::accessToken()
{
    QMap<QString, QString> headerParameters;
    headerParameters.insert("oauth_token", oAuthToken_);
    headerParameters.insert("oauth_verifier", oAuthVerifier_);
    headerParameters.insert("oauth_consumer_key", consumerKey_);
    headerParameters.insert("oauth_nonce", nonce());
    headerParameters.insert("oauth_timestamp", QString::number(QDateTime::currentDateTimeUtc().toTime_t()));
    headerParameters.insert("oauth_version", "1.0");
    headerParameters.insert("oauth_signature_method", "HMAC-SHA1");
    headerParameters.insert("oauth_signature", sign("POST", accessTokenURL_, headerParameters));

    shr_->reset();
    shr_->setRawHeader("Authorization", buildAuthorizationHeader(headerParameters));

    QMap<QString, QString> response = parseResponse(shr_->post(accessTokenURL_, QByteArray()));

    oAuthToken_       = response.value("oauth_token");
    oAuthTokenSecret_ = response.value("oauth_token_secret");

    settings_->setValue("oAuthToken", oAuthToken_);
    settings_->setValue("oAuthVerifier", oAuthVerifier_);
    settings_->setValue("oAuthTokenSecret", oAuthTokenSecret_);
}

void O1::initServer()
{
    connect(&server_, SIGNAL(newConnection()), this, SLOT(onIncomingConnection()));
    connect(&serverTimer_, SIGNAL(timeout()), &serverLoop_, SLOT(quit()));
    serverTimer_.setInterval(TIMERINTERVAL);
}

QMap<QString, QString> O1::startServer()
{
    //Try to listen until it's possible
    int time = 0;
    while ((!server_.listen(QHostAddress::Any, PORT)) && (time < TIMERINTERVAL)) {
        QThread::msleep(100);
        time += 100;
    }
    if (time >= TIMERINTERVAL) {
        queryParams_.clear();
        return queryParams_;
    }
    //Start timer and wait for answer
    serverTimer_.setSingleShot(true);
    serverTimer_.start();
    serverLoop_.exec();
    return queryParams_;
}

void O1::onIncomingConnection()
{
    QTcpSocket* socket = server_.nextPendingConnection();
    connect(socket, SIGNAL(readyRead()), this, SLOT(onBytesReady()), Qt::UniqueConnection);
    connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
}

void O1::onTimeOut()
{
    serverLoop_.quit();
}

void O1::onBytesReady()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (!socket) {
        return;
    }

    QByteArray reply;
    QByteArray content;

    content.append(QString("<html>\n"
                   "<head>\n"
                   "<meta http-equiv=\"refresh\" content=\"0; url=http://rabiche.igk.es/thanks.php\" />"
                   "</head>\n"
                   "<body>\n"
                   "   <h1>Please wait...</h2>\n"
                   "</body>\n"
                   "</html>\n"));
    reply.append("HTTP/1.0 200 OK \r\n");
    reply.append("Content-Type: text/html; charset=\"utf-8\"\r\n");
    reply.append(QString("Content-Length: %1\r\n\r\n").arg(content.size()));
    reply.append(content);
    socket->write(reply);

    QByteArray data = socket->readAll();
    queryParams_ = parseQueryParams(data);
    socket->disconnectFromHost();
    server_.close();
    serverLoop_.quit();
}

QMap<QString, QString> O1::parseQueryParams(const QByteArray &data)
{
    QString splitGetLine = QString(data).split("\r\n").first();
    splitGetLine.remove("GET ");
    splitGetLine.remove("HTTP/1.1");
    splitGetLine.remove("\r\n");
    splitGetLine.prepend("http://localhost");
    QUrl getTokenUrl(splitGetLine);

    QList< QPair<QString, QString> > tokens;
#if QT_VERSION < 0x050000
    tokens = getTokenUrl.queryItems();
#else
    QUrlQuery query(getTokenUrl);
    tokens = query.queryItems();
#endif
    QMultiMap<QString, QString> queryParams;
    QPair<QString, QString> tokenPair;
    foreach (tokenPair, tokens) {
        // FIXME: We are decoding key and value again. This helps with Google OAuth, but is it mandated by the standard?
        QString key = QUrl::fromPercentEncoding(QByteArray().append(tokenPair.first.trimmed()));
        QString value = QUrl::fromPercentEncoding(QByteArray().append(tokenPair.second.trimmed()));
        queryParams.insert(key, value);
    }
    return queryParams;
}
