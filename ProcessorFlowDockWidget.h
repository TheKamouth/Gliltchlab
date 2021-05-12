#ifndef PROCESSORFLOWDOCKWIDGET_H
#define PROCESSORFLOWDOCKWIDGET_H

#include "ProcessorCommonUI.h"
#include "ProcessorFlow.h"
#include "ImageProcessors/ImageProcessorBase.h"
#include "Nodes/NodeFactory.h"
#include "FlowGraph.h"

#include <QDockWidget>

namespace Ui {
class ProcessorFlowDockWidget;
}

class ProcessorFlowDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit ProcessorFlowDockWidget(QWidget *parent = nullptr);
    ~ProcessorFlowDockWidget();

    void OnAddNodeClicked();
    void OnProcessFlowClicked();

signals:
    void InputLoaded(QImage* input, int flowIndex);
    void OutputProcessed(QImage * output);
    void PeakNode(Node * node);

private:
    Ui::ProcessorFlowDockWidget *ui;

    QList<ProcessorCommonUI *> _processorsUIs;
    QList<Node *> _nodeWidgets;

    // WIP: Replace ProcessorFlow by more generic GraphFlow that handles Node (input, filters, output, ...)
    ProcessorFlow * _processorFlow;
    FlowGraph * _flowGraph;

    NodeFactory _nodeFactory;

    QImage * _input;
    QString _inputFileName;
    QImage * _output;

    void InitNodeTypeComboBox();
    void DeleteNode(Node* node);
    void OnPeakNodeClicked(Node * node);

};

#endif // PROCESSORFLOWDOCKWIDGET_H
