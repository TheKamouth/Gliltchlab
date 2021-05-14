#include "TimeControlWidget.h"
#include "ui_TimeControlWidget.h"

TimeControlWidget::TimeControlWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::TimeControlWidget)
{
    ui->setupUi(this);
}

TimeControlWidget::~TimeControlWidget()
{
    delete ui;
}
