#include "editsegmentationwidget.h"
#include "ui_editsegmentationwidget.h"

EditSegmentationWidget::EditSegmentationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditSegmentationWidget)
{
    ui->setupUi(this);

    QObject::connect(ui->slider_threshold, &QSlider::valueChanged, ui->sb_threshold, &QSpinBox::setValue);
}

EditSegmentationWidget::~EditSegmentationWidget()
{
    QObject::disconnect(ui->slider_threshold, &QSlider::valueChanged, ui->sb_threshold, &QSpinBox::setValue);
    delete ui;
}

int EditSegmentationWidget::GetThreshold()
{
    return ui->sb_threshold->value();
}

bool EditSegmentationWidget::IsThresholdSegmentationEnabled()
{
    return ui->gb_threshold->isChecked();
}

bool EditSegmentationWidget::IsFunctionSegmentationEnabled()
{
    return false;//return ui->gb_function->isChecked();
}

int EditSegmentationWidget::GetSegConstant()
{
    return 0;//return ui->sb_B->value();
}

int EditSegmentationWidget::GetSegSlope()
{
    return 0;//return ui->sb_A->value();
}

int EditSegmentationWidget::GetRandMin()
{
    return 0;//return ui->sb_rand_min->value();
}

int EditSegmentationWidget::GetRandMax()
{
    return 0;//return ui->sb_rand_max->value();
}

void EditSegmentationWidget::CircularSortingEnabled(bool enable)
{
    ui->gb_threshold->setEnabled(!enable);
}
