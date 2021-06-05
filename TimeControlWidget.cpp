#include "TimeControlWidget.h"
#include "ui_TimeControlWidget.h"

TimeControlWidget::TimeControlWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::TimeControlWidget)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::Tool);

    QObject::connect(ui->pb_firstFrame, &QPushButton::clicked, this, &TimeControlWidget::OnFirstFrameButtonClicked);
    QObject::connect(ui->pb_backwardOneFrame, &QPushButton::clicked, this, &TimeControlWidget::OnBackwardOneFrameButtonClicked);
    QObject::connect(ui->pb_playPause, &QPushButton::clicked, this, &TimeControlWidget::OnPlayPauseButtonClicked);
    QObject::connect(ui->pb_forwardOneFrame, &QPushButton::clicked, this, &TimeControlWidget::OnForwardOneFrameButtonClicked);
    QObject::connect(ui->pb_lastFrame, &QPushButton::clicked, this, &TimeControlWidget::OnLastFrameButtonClicked);
}

TimeControlWidget::~TimeControlWidget()
{
    delete ui;
}

void TimeControlWidget::OnFirstFrameButtonClicked()
{

}

void TimeControlWidget::OnBackwardOneFrameButtonClicked()
{

}

void TimeControlWidget::OnPlayPauseButtonClicked()
{

}

void TimeControlWidget::OnForwardOneFrameButtonClicked()
{

}

void TimeControlWidget::OnLastFrameButtonClicked()
{

}
