#include "PixelSortFilterNode.h"
#include "ui_PixelSortFilterNode.h"


PixelSortFilterNode::PixelSortFilterNode() :
    Node(),
    ui(new Ui::PixelSortFilterNode)
{
    ui->setupUi(this);

    directionEditor = new EditDirectionWidget();
    SpecificUiLayout()->layout()->addWidget(directionEditor);

    functionEditor = new EditFunctionWidget();
    SpecificUiLayout()->layout()->addWidget(functionEditor);

    segmentationEditor = new EditSegmentationWidget();
    SpecificUiLayout()->layout()->addWidget(segmentationEditor);

    QObject::connect(directionEditor, &EditDirectionWidget::CircularSortingEnabled, this, &Node::EmitNodeInputChanged<bool>);

    // TODO
    //_gliltchLabCore->moveToThread( &workerThread);
    //_gliltchLabCore.start();
}

PixelSortFilterNode::~PixelSortFilterNode()
{
    delete ui;
}

QWidget * PixelSortFilterNode::NodeUiBaseWidgetInForm()
{
    return ui->NodeUiBase;
}

QLayout * PixelSortFilterNode::NodeUiBaseLayoutInForm()
{
    return ui->NodeCommonWidgetLayout;
}

QWidget * PixelSortFilterNode::SpecificUI()
{
    return ui->widget;
}

QLayout * PixelSortFilterNode::SpecificUiLayout()
{
    return ui->NodeSpecificWidgetLayout;
}

bool PixelSortFilterNode::BeforeProcessing()
{
    return true;
}

bool PixelSortFilterNode::ProcessInternal()
{
    //TODO: utiliser l'image de la tabcourante!
    _gliltchLabCore.SetInputImage(_input);

    //gliltchLabCore->SetInputImage( inputFileName);

    _gliltchLabCore.SetSortingAngle( directionEditor->GetSortingAngle() );
    _gliltchLabCore.SetColorToValueFunction( (SortingRule) functionEditor->GetSortingRuleId() );
    _gliltchLabCore.InvertSortingRule( functionEditor->isInvertedRule());
    _gliltchLabCore.SetCircularSorting( directionEditor->IsCircularSorting());
    _gliltchLabCore.SetSegmentationRule( segmentationEditor->IsThresholdSegmentationEnabled(), segmentationEditor->GetThreshold());
    _gliltchLabCore.SetCircularSortingCenter( QPointF( 0.5f, 0.5f));

    //OpenGLWidget * currentOpenGLWIdget = _tabWidgetImageViewer->CurrentOpenGLWidget();
    //if(currentOpenGLWIdget)
        //gliltchLabCore->SetCircularSortingCenter( currentOpenGLWIdget->getSortingCenter());

    _gliltchLabCore.ProcessGlitchRefactor();

    _output = _gliltchLabCore.GetOuputImage();

    return true;
}
