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

    QObject::connect(node->CommonWidget(), &NodeCommonWidget::DeleteClicked, this, &ProcessorFlowDockWidget::DeleteNode);
    QObject::connect(node->CommonWidget(), &NodeCommonWidget::PeakClicked, this, &ProcessorFlowDockWidget::OnPeakNodeClicked);

    //ui->vboxLayoutProcessors->addWidget(node->Widget());
    ui->vboxLayoutProcessors->insertWidget(_nodeWidgets.count(), node->Widget());

    // Should Node be a QWidget ? Or handle one ?
    _nodeWidgets.append(node);
    _flowGraph->AddNode(node);
}

void ProcessorFlowDockWidget::DeleteNode(Node* node)
{
    QObject::disconnect(node->CommonWidget(), &NodeCommonWidget::DeleteClicked, this, &ProcessorFlowDockWidget::DeleteNode);

    _flowGraph->RemoveNode(_nodeWidgets.indexOf(node));
    _nodeWidgets.removeAll(node);

    delete node;
}

void ProcessorFlowDockWidget::OnPeakNodeClicked(Node * node)
{
    for(int i = 0 ; i < _nodeWidgets.count(); i++ )
    {
        Node * n = _nodeWidgets.at(i);
        if(n != node)
        {
            n->CommonWidget()->SetIsPeaked(false);
        }
    }

    emit PeakNode(node);
}

void ProcessorFlowDockWidget::InitNodeTypeComboBox()
{
    QStringList availableNodeTypesNames = _nodeFactory.AvailableNodeTypesNames();
    ui->cb_processorType->addItems(availableNodeTypesNames);
}

void ProcessorFlowDockWidget::OnProcessFlowClicked()
{
    if(_processorsUIs.count() == 0)
    {
        qDebug() << "No effect to process." << Qt::endl;
        return;
    }

    if(_input->isNull())
    {
        qDebug() << "No valid input to process." << Qt::endl;
        return;
    }

    for(int i = 0 ; i < _processorsUIs.count() ; i++)
    {
        ImageProcessorBase * processor = _processorsUIs[i]->Processor();
        processor->SetInput(_input);
        processor->Process();
        _output = new QImage(*processor->Output());
    }

    emit OutputProcessed(_output);
}
