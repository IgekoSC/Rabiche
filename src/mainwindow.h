#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebView>
#include <QSplitter>
#include <Twitter/twitter.h>
#include <twitterhtmlbridge.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Twitter* twitter_;
    QThread* thread_;
    QWebView* webView_;
    TwitterHtmlBridge* htmlBridge_;

};

#endif // MAINWINDOW_H
