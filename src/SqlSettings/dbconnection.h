#ifndef DBCONNECTION_H
#define DBCONNECTION_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlField>
#include <QDateTime>
#include <QMessageBox>
#include <QDesktopServices>
#include <QDir>

typedef QMap<QString, QVariant> SqlRow;

inline static bool ConecToDb(QSqlDatabase &Db, QString dbName)
{
    //Obtenemos el directorio en el que se almacenan los datos de las aplicaciones
    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    QDir dir(dataDir);
    dir.mkpath(dir.path());
    //Creamos la ruta al fichero de la base de datos
    dbName = dir.filePath(dbName+".sqlite");

    //Instanciamos QSqlDatabase y utilizamos dbName como ruta
    //al archivo de la base de datos
    Db = QSqlDatabase::addDatabase("QSQLITE", dbName);
    Db.setDatabaseName(dbName);

    //Abre la base de datos y muestra un aviso en caso de error
    if (!Db.open()) {
      QMessageBox::critical(0, "Error de conexión la base de datos.", Db.lastError().text());
      return false;
    }

    return true;
}

inline static bool ConecToDb(QSqlDatabase &Db, QString HostName, QString DatabaseName, QString UserName, QString Password, QString driver = "QMYSQL"){
    Db = QSqlDatabase::addDatabase(driver);
    Db.setHostName(HostName);
    Db.setDatabaseName(DatabaseName);
    Db.setUserName(UserName);
    Db.setPassword(Password);
    if (!Db.open()) {
        QMessageBox::critical(0, "Error de conexión la base de datos.", Db.lastError().text());
        return false;
    }
    return true;
}

inline static const QVariant GetField(QSqlQuery &Query, QString Field) {
    return Query.record().field(Field).value();
}

inline static void RemoveDb() {
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
}

#endif // DBCONNECTION_H
