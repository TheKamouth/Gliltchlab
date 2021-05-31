#ifndef PROCESSORFLOWDOCKWIDGET_H
#define PROCESSORFLOWDOCKWIDGET_H

#include "Nodes/FilterNodes/ImageProcessorBase.h"
#include "Nodes/NodeFactory.h"
#include "FlowGraph.h"

#include <QDockWidget>
#include <QThread>
/*
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
    void OnNodeAdded(INode * node);
    void AddNodeWidget(INode * node);

    void OnProcessFlowClicked();
    void OnPeakNodeClicked(INode * node);
    void PeakLastFlowNode();

    FlowGraph * CurrentFlowGraph() { return _flowGraph;};

signals:
    void InputLoaded(QImage* input, int flowIndex);
    void OutputProcessed(QImage * output);
    void PeakNode(INode * node);

private:
    Ui::ProcessorFlowDockWidget *ui;

    //QList<Node *> _nodes;
    FlowGraph * _flowGraph;
    //NodeFactory _nodeFactory;

    QImage * _input;
    QString _inputFileName;
    QImage * _output;

    void InitNodeTypeComboBox();
    void DeleteNode(INode* node);

};
*/
#endif // PROCESSORFLOWDOCKWIDGET_H
