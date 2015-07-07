TARGET = rabiche
TEMPLATE = app

QT += qml quick widgets sql network

SOURCES += main.cpp \
    tweet.cpp \
    twitter.cpp \
    twitterentities.cpp \
    twitterentity.cpp \
    twitterhashtag.cpp \
    twittermedia.cpp \
    twitterstream.cpp \
    twittersymbol.cpp \
    twitterurl.cpp \
    twitteruser.cpp \
    twitterusermention.cpp \
    twittwidget.cpp \
    SimpleCrypt/simplecrypt.cpp \
    SqlSettings/sqlsettings.cpp \
    SynchronousHttpRequest/synchronoushttprequest.cpp \
    ThreadManager/threadmanager.cpp \
    ThreadManager/worker.cpp \
    o1/o1.cpp \
    o1/o1twitter.cpp \
    o1/signedrequester.cpp \
    rabiche.cpp

HEADERS += \
    tweet.h \
    twitter.h \
    twitterentities.h \
    twitterentity.h \
    twitterhashtag.h \
    twittermedia.h \
    twitterstream.h \
    twittersymbol.h \
    twitterurl.h \
    twitteruser.h \
    twitterusermention.h \
    twittwidget.h \
    SimpleCrypt/simplecrypt.h \
    SqlSettings/dbconnection.h \
    SqlSettings/sqlsettings.h \
    SynchronousHttpRequest/synchronoushttprequest.h \
    ThreadManager/threadmanager.h \
    ThreadManager/worker.h \
    o1/o1.h \
    o1/o1twitter.h \
    o1/signedrequester.h \
    rabiche.h

RESOURCES += \
    Resources.qrc

CODECFORTR = UTF-8

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

#Build config
DESTDIR = ../bin
MOC_DIR = ../build/moc
RCC_DIR = ../build/rcc
UI_DIR = ../build/ui
unix:OBJECTS_DIR = ../build/o/unix
win32:OBJECTS_DIR = ../build/o/win32
macx:OBJECTS_DIR = ../build/o/mac

# Default rules for deployment.
include(deployment.pri)
