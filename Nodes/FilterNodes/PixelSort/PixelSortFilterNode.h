#ifndef PIXELSORTFILTERNODE_H
#define PIXELSORTFILTERNODE_H

#include "Nodes/FilterNodes/PixelSort/ui/editdirectionwidget.h"
#include "Nodes/FilterNodes/PixelSort/ui/editfunctionwidget.h"
#include "Nodes/FilterNodes/PixelSort/ui/editmaskwidget.h"
#include "Nodes/FilterNodes/PixelSort/ui/editsegmentationwidget.h"
#include "Nodes/FilterNodes/PixelSort/core/glilcthlabcore.h"

#include "Nodes/Node.h"

#include <QWidget>

namespace Ui {
class PixelSortFilterNode;
}

class PixelSortFilterNode : public Node
{
    Q_OBJECT

public:
    explicit PixelSortFilterNode();
    ~PixelSortFilterNode();

    // Node
    virtual QString Name() override { return "Pixel sort"; }
    virtual QWidget * Widget() override { return this; };
    virtual QWidget * NodeUiBaseWidgetInForm() override;
    virtual QLayout * NodeUiBaseLayoutInForm() override;
    virtual QWidget * SpecificUI() override;
    virtual QLayout * SpecificUiLayout() override;
    virtual bool BeforeProcessing() override;
    virtual bool ProcessInternal() override;

private:
    Ui::PixelSortFilterNode *ui;

    GlilcthLabCore _gliltchLabCore;

    //Pointers to widgets allowing glitch parameterization
    EditDirectionWidget * directionEditor;
    EditFunctionWidget * functionEditor;
    EditMaskWidget * maskEditor;
    EditSegmentationWidget * segmentationEditor;

    void OnInputChanged();
};

#endif // PIXELSORTFILTERNODE_H
