#include "ProcessorCommonUI.h"
#include "ui_ProcessorCommonUI.h"

//#include "ImageProcessors/ImageProcessorBase.h";

ProcessorCommonUI::ProcessorCommonUI(ImageProcessorBase * processor, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProcessorCommonUI),
    _processor(processor)
{
    ui->setupUi(this);

    QObject::connect( ui->pb_delete, &QPushButton::clicked, this, &ProcessorCommonUI::OnDeleteProcessorClicked);
    QObject::connect( ui->pb_settings, &QPushButton::clicked, this, &ProcessorCommonUI::OnProcessorSettingsClicked);
    QObject::connect( ui->cb_enabled, &QPushButton::clicked, this, &ProcessorCommonUI::OnEnableProcessorCheckboxClicked);
    QObject::connect( ui->pb_peak, &QPushButton::clicked, this, &ProcessorCommonUI::OnPeakClicked);
    QObject::connect( ui->pb_arrow, &QPushButton::clicked, this, &ProcessorCommonUI::OnArrowClicked);

    ui->le_name->text() = processor->Name();

    _isVisible = false;
}

ProcessorCommonUI::~ProcessorCommonUI()
{
    delete ui;
}

ImageProcessorBase * ProcessorCommonUI::Processor()
{
    return _processor;
}

void ProcessorCommonUI::OnProcessorSettingsClicked()
{
    // TODO
}

void ProcessorCommonUI::OnDeleteProcessorClicked()
{
    emit DeleteClicked(this);
}

void ProcessorCommonUI::OnEnableProcessorCheckboxClicked()
{
    _processor->Enable(ui->cb_enabled->isChecked());

    emit EnableClicked(this,ui->cb_enabled->isChecked());
}

void ProcessorCommonUI::OnPeakClicked()
{
    // TODO
}

void ProcessorCommonUI::SetWidgetVisible(bool visible)
{
    if(visible)
    {
        ui->widget->setMaximumHeight(0);
    }
    else
    {
        ui->widget->setMaximumHeight(200);
    }
}

void ProcessorCommonUI::OnArrowClicked()
{
    _isVisible = ! _isVisible;
    SetWidgetVisible(_isVisible);
}
