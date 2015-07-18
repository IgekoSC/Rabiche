TARGET = rabiche
TEMPLATE = app

QT += qml quick widgets sql network

CONFIG += c++11

SOURCES += main.cpp \
    Twitter/tweet.cpp \
    Twitter/twitter.cpp \
    Twitter/twitterentities.cpp \
    Twitter/twitterentity.cpp \
    Twitter/twitterhashtag.cpp \
    Twitter/twittermedia.cpp \
    Twitter/twitterstream.cpp \
    Twitter/twittersymbol.cpp \
    Twitter/twitterurl.cpp \
    Twitter/twitteruser.cpp \
    Twitter/twitterusermention.cpp \
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
    Twitter/tweet.h \
    Twitter/twitter.h \
    Twitter/twitterentities.h \
    Twitter/twitterentity.h \
    Twitter/twitterhashtag.h \
    Twitter/twittermedia.h \
    Twitter/twitterstream.h \
    Twitter/twittersymbol.h \
    Twitter/twitterurl.h \
    Twitter/twitteruser.h \
    Twitter/twitterusermention.h \
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
