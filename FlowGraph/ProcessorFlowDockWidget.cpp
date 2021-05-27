#include "ProcessorFlowDockWidget.h"
#include "ui_ProcessorFlowDockWidget.h"

#include "Nodes/NodeCommonWidget.h"

#include <QFileDialog>
#include <QDebug>

ProcessorFlowDockWidget::ProcessorFlowDockWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::ProcessorFlowDockWidget)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::Tool | Qt::WindowTitleHint);

    QObject::connect( ui->pb_addProcessor, &QPushButton::clicked, this, &ProcessorFlowDockWidget::OnAddNodeClicked);
    QObject::connect( ui->pb_processFlow, &QPushButton::clicked, this, &ProcessorFlowDockWidget::OnProcessFlowClicked);

    InitNodeTypeComboBox();

    _flowGraph = new FlowGraph();

    QObject::connect( _flowGraph, &FlowGraph::NodeAdded, this, &ProcessorFlowDockWidget::OnNodeAdded);
}

ProcessorFlowDockWidget::~ProcessorFlowDockWidget()
{
    delete ui;
}

void ProcessorFlowDockWidget::OnAddNodeClicked()
{
    // Get selected NodeType
    int currentIndex = ui->cb_processorType->currentIndex();
    NodeType nodeType = (NodeType)currentIndex;

    // Create Node and add to flow
    Node* node = _flowGraph->AddNode(nodeType);

    OnProcessFlowClicked();

    OnPeakNodeClicked(node);
}

void ProcessorFlowDockWidget::OnNodeAdded(Node * node)
{
    AddNodeWidget( node);
}

void ProcessorFlowDockWidget::AddNodeWidget(Node *node)
{
    if(node == nullptr)
    {
        qDebug() << __FUNCTION__ << " node is nullptr";
        return;
    }

    node->InitializeNodeCommonWidget();

    QObject::connect(node->CommonWidget(), &NodeCommonWidget::DeleteClicked, this, &ProcessorFlowDockWidget::DeleteNode);
    QObject::connect(node->CommonWidget(), &NodeCommonWidget::PeakClicked, this, &ProcessorFlowDockWidget::OnPeakNodeClicked);

    ui->vboxLayoutProcessors->insertWidget(_flowGraph->NodeCount() - 1, node->Widget());
}

void ProcessorFlowDockWidget::DeleteNode(Node* node)
{
    QObject::disconnect(node->CommonWidget(), &NodeCommonWidget::DeleteClicked, this, &ProcessorFlowDockWidget::DeleteNode);

    _flowGraph->RemoveNode(node);
}

void ProcessorFlowDockWidget::OnPeakNodeClicked(Node * node)
{
    node->CommonWidget()->SetIsPeakedAt(true);

    /*
    for(int i = 0 ; i < _nodes.count(); i++ )
    {
        Node * n = _nodes.at(i);
        if(n != node)
        {
            n->CommonWidget()->SetIsPeaked(false);
        }
    }
    */

    emit PeakNode(node);
}

void ProcessorFlowDockWidget::PeakLastFlowNode()
{
    int nodeCount = _flowGraph->NodeCount();
    Node * lastNode = _flowGraph->GetNode(nodeCount - 1);
    OnPeakNodeClicked(lastNode);
}

void ProcessorFlowDockWidget::InitNodeTypeComboBox()
{
    QStringList availableNodeTypesNames = NodeFactory::AvailableNodeTypesNames();
    ui->cb_processorType->addItems(availableNodeTypesNames);
}

void ProcessorFlowDockWidget::OnProcessFlowClicked()
{
    _flowGraph->Process();

    emit OutputProcessed(_flowGraph->Output());
}
