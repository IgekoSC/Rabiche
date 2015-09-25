#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebView>
#include <QSplitter>
#include <QDockWidget>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <Twitter/twitter.h>
#include "twitterhtmlbridge.h"
#include "wgtnewtweet.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QSqlDatabase db_;
    SqlSettings* settings_;
    Twitter* twitter_;
    QThread* thread_;
    QWebView* webView_;
    TwitterHtmlBridge* htmlBridge_;

    //GUI
    QDockWidget* newTweetDock_;
    WgtNewTweet* wgtNewTweet_;
    QMenu* mnuWindows_;
    QAction* actShowNewTweet_;

};

#endif // MAINWINDOW_H
