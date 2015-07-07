#include <SqlSettings/sqlsettings.h>

SqlSettings::SqlSettings(QSqlDatabase *Db, QObject *parent) :
    QObject(parent)
{
    crypt_.setKey(0xed04d69cd182b5e7);

    secure_ = true;

    Db_ = Db;

    Db_->exec("CREATE TABLE IF NOT EXISTS config ("
              "key TEXT,"
              "value TEXT"
              ");");

    Db_->exec("CREATE UNIQUE INDEX IF NOT EXISTS \"key\" ON config (key ASC);");
}

void SqlSettings::setValue(const QString & key, const QVariant & value)
{
    QString sValue;
    if (secure_)
        sValue = crypt_.encryptToString(value.toString());
    else
        sValue = value.toString();

    Db_->exec("INSERT OR REPLACE INTO config (key, value) "
              "VALUES ( \"" + key + "\", \"" + sValue + "\");");
}

QVariant SqlSettings::value(const QString & key, const QVariant & defaultValue)
{
    QSqlQuery q = Db_->exec("SELECT value "
                            "FROM config "
                            "WHERE key=\"" + key + "\";");

    q.first();

    QVariant tmp = q.value("value");

    if (tmp.isValid()) {
        if (secure_)
            return QVariant::fromValue(crypt_.decryptToString(tmp.toString()));
        else
            return QVariant::fromValue(tmp.toString());
    }

    return defaultValue;
}

bool SqlSettings::secure() const
{
    return secure_;
}

void SqlSettings::setSecure(bool secure)
{
    secure_ = secure;
}

