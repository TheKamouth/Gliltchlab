#include "ProcessorFlowDockWidget.h"
#include "ui_ProcessorFlowDockWidget.h"

#include "Nodes/NodeCommonWidget.h"

#include "ImageProcessors/ContrastProcessor.h"
#include "ImageProcessors/Scanner.h"

#include <QFileDialog>


ProcessorFlowDockWidget::ProcessorFlowDockWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::ProcessorFlowDockWidget)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::Tool);

    QObject::connect( ui->pb_addProcessor, &QPushButton::clicked, this, &ProcessorFlowDockWidget::OnAddNodeClicked);
    QObject::connect( ui->pb_processFlow, &QPushButton::clicked, this, &ProcessorFlowDockWidget::OnProcessFlowClicked);

    InitNodeTypeComboBox();
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
    Node* node = _nodeFactory.CreateNode(nodeType);

    if(node == nullptr)
    {
        return;
    }

    node->Initialize();

    QObject::connect(node->CommonWidget(), &NodeCommonWidget::DeleteClicked, this, &ProcessorFlowDockWidget::DeleteNode);
    QObject::connect(node->CommonWidget(), &NodeCommonWidget::PeakClicked, this, &ProcessorFlowDockWidget::OnPeakNodeClicked);

    //ui->vboxLayoutProcessors->addWidget(node->Widget());
    ui->vboxLayoutProcessors->insertWidget(_nodes.count(), node->Widget());

    // Should Node be a QWidget ? Or handle one ?
    _nodes.append(node);
    _flowGraph->AddNode(node);

    OnProcessFlowClicked();
}

void ProcessorFlowDockWidget::DeleteNode(Node* node)
{
    QObject::disconnect(node->CommonWidget(), &NodeCommonWidget::DeleteClicked, this, &ProcessorFlowDockWidget::DeleteNode);

    _flowGraph->RemoveNode(_nodes.indexOf(node));
    _nodes.removeAll(node);

    delete node;
}

void ProcessorFlowDockWidget::OnPeakNodeClicked(Node * node)
{
    for(int i = 0 ; i < _nodes.count(); i++ )
    {
        Node * n = _nodes.at(i);
        if(n != node)
        {
            n->CommonWidget()->SetIsPeaked(false);
        }
    }

    emit PeakNode(node);
}

void ProcessorFlowDockWidget::PeakLastFlowNode()
{
    int nodeCount = _nodes.count();
    Node * lastNode = _nodes[nodeCount - 1];
    OnPeakNodeClicked(lastNode);
}

void ProcessorFlowDockWidget::InitNodeTypeComboBox()
{
    QStringList availableNodeTypesNames = _nodeFactory.AvailableNodeTypesNames();
    ui->cb_processorType->addItems(availableNodeTypesNames);
}

void ProcessorFlowDockWidget::OnProcessFlowClicked()
{
    if(_nodes.count() == 0)
    {
        qDebug() << "No node to process." << Qt::endl;
        return;
    }

    if(_nodes.count() == 1)
    {
        emit OutputProcessed(_nodes[0]->Output());
        return;
    }

    Node * previousNode = nullptr;
    Node * lastNode = nullptr;
    Node * node;
    for(int i = 1 ; i < _nodes.count() ; i++)
    {
        previousNode = _nodes[i-1];
        node = _nodes[i];
        node->SetInput( previousNode->Output());

        if( node->TryProcess() == false)
        {
            qDebug() << "Failed to process node: " << node->Name();
        }
    }

    lastNode = node;
    emit OutputProcessed(lastNode->Output());
}
