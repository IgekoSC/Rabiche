#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("Igeko");
    QCoreApplication::setOrganizationDomain("igeko.es");
    QCoreApplication::setApplicationName("Rabiche");
    QApplication a(argc, argv);

    MainWindow mainWindow;
    //Center window
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = (screenGeometry.width()-mainWindow.width()) / 2;
    int y = (screenGeometry.height()-mainWindow.height()) / 2;
    mainWindow.move(x, y);

    mainWindow.show();

    return a.exec();
}
