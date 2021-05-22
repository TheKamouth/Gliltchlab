#include "Node.h"

#include "NodeCommonWidget.h"

#include <QLayout>
#include <QDateTime>
#include <QDebug>
#include <QElapsedTimer>

Node::Node()
{
    _isEnabled = true;
    _nodeCommonWidget = nullptr;
    _peakWidget = nullptr;
    _flowGraphPosition = -1;

    // Could be handled by GenericNode
    _input = nullptr;
    _output = nullptr;
}

Node::~Node()
{
    delete _nodeCommonWidget;

    if(_peakWidget != nullptr)
    {
        delete _peakWidget;
    }
}

bool Node::TryProcess()
{
    // If connection has changed (meaning depends on ConnectionType)
    // ReadConnections

    qDebug() << "### "<< __FUNCTION__<< " " << Name();

    if(BeforeProcessing() == false)
    {
         qDebug() << "Failed to process " << Name();
        return false;
    }

    QString processingLogOutput = "";
    QElapsedTimer processingTimer;
    processingTimer.start();
    int processingTime;

    if(_isEnabled)
    {
        ProcessInternal();

        processingTime = processingTimer.elapsed();
        processingLogOutput += "took " + QString::number(processingTime) + "ms to process.";
    }
    else
    {
        _output = new QImage(*_input);

        processingTime = processingTimer.elapsed();
        processingLogOutput += "took " + QString::number(processingTime) + "ms (disabled) to process.";
    }


    if(_output == nullptr)
    {
        qDebug() << "_ouput is null";
        return false;
    }

    update();
    CommonWidget()->OnPeakClicked();

    AfterProcessing();

    // WriteConnections

    CommonWidget()->ShowLastProcessingTime(processingTime);
    qDebug() << processingLogOutput;
    qDebug() << "----------------------";

    emit NodeOutputChanged(this);

    return true;
}

bool Node::BeforeProcessing()
{
    if(_input == nullptr)
    {
        qDebug() << "Cannot save: _input is null";
        return false;
    }

    return true;
}

bool Node::AfterProcessing()
{
    return true;
}

bool Node::ProcessInternal()
{
    _output = new QImage(*_input);

    return true;
}

void Node::InitializeNodeCommonWidget()
{
    // Cannot be done in constructor as NodeCommonWidget constructor relies on dynamic type
    _nodeCommonWidget = new NodeCommonWidget(this);

    QObject::connect( _nodeCommonWidget, &NodeCommonWidget::DeleteClicked, this, &Node::OnDeleteNodeClicked);

    NodeUiBaseLayoutInForm()->addWidget(_nodeCommonWidget);

    _nodeCommonWidget->setVisible(true);
}

void Node::OnEnableNodeCheckboxClicked(bool toggled)
{
    SpecificUI()->setEnabled(toggled);

    _isEnabled = toggled;

    emit NodeInputChanged(this);
}

void Node::OnExpandCollapseArrowClicked(bool isSpecificWidgetExpanded)
{
    SpecificUI()->setVisible(isSpecificWidgetExpanded);
}

QString Node::GetTempImageOutputFilePath()
{
    QString nodeName = Name();
    QString timeStamp = QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss_z");
    return "nodeName_image_output" + timeStamp + ".png";
}

void Node::EmitNodeInputChanged()
{
    emit NodeInputChanged(this);
}

int Node::FlowGraphNodePosition()
{
    return _flowGraphPosition;
}

