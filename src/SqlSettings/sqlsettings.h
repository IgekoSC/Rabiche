#ifndef SQLSETTINGS_H
#define SQLSETTINGS_H

#include <QObject>
#include <QVariant>
#include <QDebug>
#include <SimpleCrypt/simplecrypt.h>
#include "dbconnection.h"

class SqlSettings : public QObject
{
    Q_OBJECT
public:
    explicit SqlSettings(QSqlDatabase* Db, QObject *parent = 0);
    void setValue(const QString &key, const QVariant &value);
    QVariant value(const QString &key, const QVariant &defaultValue = QVariant());
    bool secure() const;
    void setSecure(bool secure);

private:
    QSqlDatabase* Db_;
    SimpleCrypt crypt_;
    bool secure_;

};

#endif // SQLSETTINGS_H
