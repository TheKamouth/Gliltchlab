#include "TimelineWidget.h"
#include "ui_TimelineWidget.h"

#include "TimeManager.h"

#include <QKeyEvent>

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

void TimelineWidget::SetTimeline(QWidget * timeline)
{
    timeline->setParent(this);
    timeline->show();
    ui->dockWidgetContents->layout()->addWidget(timeline);
}

void TimelineWidget::keyPressEvent(QKeyEvent *event)
{
    if( event->key() == Qt::Key_Space)
    {
        TimeManager::Instance().PlayPause();
    }
}
