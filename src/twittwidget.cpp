#include "twittwidget.h"

TwittWidget::TwittWidget(QWidget *parent) : QWidget(parent)
{
    layout_ = new QGridLayout(this);
    this->setLayout(layout_);

    text_ = new QLabel("Hola mundo", this);
    layout_->addWidget(text_);
}

TwittWidget::~TwittWidget()
{

}

