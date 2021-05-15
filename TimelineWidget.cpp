#include "TimelineWidget.h"
#include "ui_TimelineWidget.h"

TimelineWidget::TimelineWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::TimelineWidget)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Tool);
}

TimelineWidget::~TimelineWidget()
{
    delete ui;
}
