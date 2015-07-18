#ifndef TWITTWIDGET_H
#define TWITTWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <Twitter/tweet.h>

class TwittWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TwittWidget(QWidget *parent = 0);
    ~TwittWidget();

private:
    QGridLayout *layout_;
    QLabel *text_;
};

#endif // TWITTWIDGET_H
