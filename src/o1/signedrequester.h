#ifndef SIGNEDREQUESTER_H
#define SIGNEDREQUESTER_H

#include <QObject>
#include <o1/o1.h>

class SignedRequester : public QObject
{
    Q_OBJECT
public:
    explicit SignedRequester(O1 *o1, QObject *parent = 0);
    QByteArray doAuthorizedRequest(SynchronousHttpRequest* shr, QString url, QMap<QString, QString> params = QMap<QString, QString>(), QString verb = "GET", QFile *fromFile = 0);
    QByteArray buildAuthorizationHeader(QString url, QMap<QString, QString> params = QMap<QString, QString>(), QString verb = "GET", QFile *fromFile = 0);

private:
    O1 *o1_;

    QString sign(QString httpMethod, QString url, QMap<QString, QString> params);

};

#endif // SIGNEDREQUESTER_H
