#include "DesaturateFilterNode.h"
#include "ui_DesaturateFilterNode.h"

#include <QStringList>

DesaturateFilterNode::DesaturateFilterNode(QWidget *parent) :
    ImagePeakNode(),
    ui(new Ui::DesaturateFilterNode)
{
    ui->setupUi(this);

    Initialize();
    InitNodeTypeComboBox();
}

DesaturateFilterNode::~DesaturateFilterNode()
{
    delete ui;
}

QWidget * DesaturateFilterNode::NodeUiBaseWidgetInForm()
{
    return ui->NodeUiBase;
}

QLayout *DesaturateFilterNode::NodeUiBaseLayoutInForm()
{
    return ui->NodeCommonWidgetLayout;
}

QWidget * DesaturateFilterNode::SpecificUI()
{
    return ui->widget;
}

QWidget *DesaturateFilterNode::InstantiatePeakWidget()
{
    // TODO
    // return glWidget with filter output shown;
}

void DesaturateFilterNode::InitNodeTypeComboBox()
{
    QStringList availableDesaturationMethodNames = AvailableDesaturationMethodNames();
    ui->cb_desaturationMethod->addItems(availableDesaturationMethodNames);
}

QStringList DesaturateFilterNode::AvailableDesaturationMethodNames()
{
    QStringList availableNodeTypesNames;

    for(int i = 0 ; i < DesaturationMethodCount ; i++)
    {
        availableNodeTypesNames.append(AvailableDesaturationMethodName(i));
    }

    return availableNodeTypesNames;
}

QString DesaturateFilterNode::AvailableDesaturationMethodName(int index)
{
    return AvailableDesaturationMethodName((DesaturationMethod)index);
}

QString DesaturateFilterNode::AvailableDesaturationMethodName(DesaturationMethod desaturationMethod)
{
    switch(desaturationMethod)
    {
        case Lightness:
            return "Lightness";

        case Average:
            return "Average";

        case Min:
            return "Min";

        case Max:
            return "Max";

        default:
            return "Default desaturation method name";
    }
}
