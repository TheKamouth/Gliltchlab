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
    qDebug() << "TryProcess: " << Name() ;

    _output = new QImage(*_input);
    return true;
}

void Node::Initialize()
{
    // Cannot be done in constructor as NodeCommonWidget constructor relies on dynamic type
    _nodeCommonWidget = new NodeCommonWidget(this);

    QObject::connect( _nodeCommonWidget, &NodeCommonWidget::DeleteClicked, this, &Node::OnDeleteProcessorClicked);

    //_nodeCommonUi->setParent(NodeUiBaseWidgetInForm());

    NodeUiBaseLayoutInForm()->addWidget(_nodeCommonWidget);

    //QWidget* nodeCommonUiWidget = NodeUiBaseWidgetInForm();

    //_nodeCommonUi->setParent(NodeUiBaseWidgetInForm());
    //QLayout* layout = _nodeCommonUi->layout();
    //layout->addWidget(NodeUiBaseWidgetInForm());

    _nodeCommonWidget->setVisible(true);
    //SpecificUI()->setVisible(false);
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
