#include "TimeControlWidget.h"
#include "ui_TimeControlWidget.h"

TimeControlWidget::TimeControlWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::TimeControlWidget)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Tool);
}

TimeControlWidget::~TimeControlWidget()
{
    delete ui;
}
