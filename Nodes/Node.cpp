#include "Node.h"

#include "NodeCommonWidget.h"

#include <QLayout>
#include <QDateTime>
#include <QDebug>

Node::Node()
{
    _isEnabled = true;
    _nodeCommonWidget = nullptr;
    _peakWidget = nullptr;

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

    BeforeProcessing();

    ProcessInternal();

    AfterProcessing();

    UpdatePeakWidget();

    // WriteConnections

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
    if(_output == nullptr)
    {
        qDebug() << "_ouput is null";
        return false;
    }

    update();

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

    QObject::connect( _nodeCommonWidget, &NodeCommonWidget::DeleteClicked, this, &Node::OnDeleteProcessorClicked);

    NodeUiBaseLayoutInForm()->addWidget(_nodeCommonWidget);

    _nodeCommonWidget->setVisible(true);
}

void Node::OnEnableProcessorCheckboxClicked(bool toggled)
{
    SpecificUI()->setEnabled(toggled);
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

void Node::EmitNodeChanged()
{
    emit NodeChanged(this);
}

int Node::FlowGraphNodePosition()
{
    return _flowGraphPosition;
}

