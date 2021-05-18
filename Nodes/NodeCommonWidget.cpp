#include "NodeCommonWidget.h"
#include "ui_NodeCommonWidget.h"

#include "Node.h"

#include <QDebug>

NodeCommonWidget::NodeCommonWidget(Node * node):
    ui(new Ui::NodeCommonWidget),
    _node(node)
{
    ui->setupUi(this);

    // Connect NodeCommonWidget
    QObject::connect( ui->pb_delete, &QPushButton::clicked, this, &NodeCommonWidget::OnDeleteNodeClicked);
    QObject::connect( ui->pb_peak, &QPushButton::clicked, this, &NodeCommonWidget::OnPeakClicked);

    //QObject::connect( ui->pb_settings, &QPushButton::clicked, node, &Node::OnProcessorSettingsClicked);
    QObject::connect( ui->cb_enabled, &QPushButton::toggled, node, &Node::OnEnableNodeCheckboxClicked);
    QObject::connect( ui->pb_arrow, &QPushButton::toggled, node, &Node::OnExpandCollapseArrowClicked);

    if(node == nullptr)
    {
        qDebug() << "NodeUiBase needs a valid node.";
    }
    else
    {
        UpdateNodeName(_node->Name());
    }

    _isExpanded = true;
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

void NodeCommonWidget::SetIsPeakedAt(bool isPeakedAt)
{
    ui->pb_peak->setChecked(isPeakedAt);
}

void NodeCommonWidget::UpdateNodeName(QString name)
{
    ui->le_name->setText(name);
}
