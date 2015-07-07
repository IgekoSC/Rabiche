#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include "rabiche.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

    // Make C++ classes available to QML.
    qmlRegisterType<Rabiche>("rabiche", 1, 0, "Rabiche");

    QQmlComponent component(&engine, QUrl(QStringLiteral("qrc:/Resources/qml/main.qml")));
    QObject* componentObj = component.create();

//    Rabiche rabiche(componentObj);

    return app.exec();

    delete componentObj;
}
