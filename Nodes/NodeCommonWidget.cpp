#include "NodeCommonWidget.h"
#include "ui_NodeUiBase.h"

#include "Node.h"

#include <QDebug>

NodeCommonWidget::NodeCommonWidget(Node * node):
    ui(new Ui::NodeUiBase),
    _node(node)
{
    ui->setupUi(this);

    // Connect NodeCommonWidget
    QObject::connect( ui->pb_delete, &QPushButton::click, this, &NodeCommonWidget::OnDeleteNodeClicked);
    QObject::connect( ui->pb_peak, &QPushButton::click, node, &NodeCommonWidget::OnPeakClicked);

    //QObject::connect( ui->pb_settings, &QPushButton::clicked, node, &Node::OnProcessorSettingsClicked);
    QObject::connect( ui->cb_enabled, &QPushButton::toggled, node, &Node::OnEnableProcessorCheckboxClicked);
    QObject::connect( ui->pb_arrow, &QPushButton::toggled, node, &Node::OnExpandCollapseArrowClicked);

    if(node == nullptr)
    {
        qDebug() << "NodeUiBase needs a valid node.";
    }
    else
    {
        UpdateNodeName(_node->Name());
    }

    _isExpanded = false;
}

NodeCommonWidget::~NodeCommonWidget()
{
    delete ui;
}

void NodeCommonWidget::OnProcessorSettingsClicked()
{
    // TODO
}

void NodeCommonWidget::OnDeleteNodeClicked()
{
    emit DeleteClicked(GetNode());
}

void NodeCommonWidget::OnEnableProcessorCheckboxClicked()
{
    _node->Enable(ui->cb_enabled->isChecked());

    emit EnableClicked(this, ui->cb_enabled->isChecked());
}

void NodeCommonWidget::OnPeakClicked()
{
    emit PeakClicked(GetNode());
}

void NodeCommonWidget::OnArrowClicked()
{
    _isExpanded = ! _isExpanded;
    SetWidgetVisible(_isExpanded);

    emit ArrowClicked(_isExpanded);
}

void NodeCommonWidget::SetWidgetVisible(bool visible)
{
}

void NodeCommonWidget::SetPeakedStatus(bool isPeakedAt)
{
    ui->pb_peak->setChecked(isPeakedAt);
}

void NodeCommonWidget::UpdateNodeName(QString name)
{
    ui->le_name->setText(name);
}
