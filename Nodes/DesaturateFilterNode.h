#ifndef CONTRASTFILTERNODE_H
#define CONTRASTFILTERNODE_H

#include "Node.h"

#include "ImagePeakNode.h"

#include <QWidget>

namespace Ui {
class DesaturateFilterNode;
}

enum DesaturationMethod
{
    Lightness,
    Average,
    Min,
    Max,
    DesaturationMethodCount
};

class DesaturateFilterNode : public ImagePeakNode
{
    Q_OBJECT

public:
    explicit DesaturateFilterNode(QWidget *parent = nullptr);
    ~DesaturateFilterNode();

    virtual QString Name() override { return "Desaturate"; }
    virtual QWidget * Widget() override { return this; };
    virtual QWidget * NodeUiBaseWidgetInForm() override;
    virtual QLayout* NodeUiBaseLayoutInForm() override;
    virtual QWidget * SpecificUI() override;
    virtual QWidget * InstantiatePeakWidget() override;

private:
    Ui::DesaturateFilterNode *ui;

    void InitNodeTypeComboBox();

    QStringList AvailableDesaturationMethodNames();
    QString AvailableDesaturationMethodName(int index);
    QString AvailableDesaturationMethodName(DesaturationMethod desaturationMethod);
};

#endif // CONTRASTFILTERNODE_H
