#ifndef PROCESSORCOMMONUI_H
#define PROCESSORCOMMONUI_H

#include "ImageProcessors/ImageProcessorBase.h"

#include <QWidget>

namespace Ui {
class ProcessorCommonUI;
}

class ProcessorCommonUI : public QWidget
{
    Q_OBJECT

public:
    explicit ProcessorCommonUI(ImageProcessorBase * processor, QWidget *parent = nullptr);
    ~ProcessorCommonUI();

    void OnProcessorSettingsClicked();
    void OnDeleteProcessorClicked();
    void OnEnableProcessorCheckboxClicked();
    void OnPeakClicked();
    void OnArrowClicked();

    ImageProcessorBase * Processor();

signals:
    void DeleteClicked(ProcessorCommonUI * processorCommonUI);
    void EnableClicked(ProcessorCommonUI * processorCommonUI, bool processorEnabled);

private:
    Ui::ProcessorCommonUI *ui;
    bool _isVisible;

    // UI is responsible for processor, is that a good thing ?
    ImageProcessorBase * _processor;

    void SetWidgetVisible(bool visible);
};

#endif // PROCESSORCOMMONUI_H
