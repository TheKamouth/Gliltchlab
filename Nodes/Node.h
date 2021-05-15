#ifndef NODE_H
#define NODE_H

#include "NodeFactory.h"

#include <QString>
#include <QWidget>

class NodeCommonWidget;

class Node : public QWidget
{
    Q_OBJECT
public:
    Node();
    ~Node();

    virtual QWidget * Widget(){ return nullptr;}

    // Node data
    virtual QString Name() { return "unnamed node"; }
    virtual void SetType( NodeType nodeType) { _nodeType = nodeType; }
    virtual NodeType Type() { return _nodeType; }
    void SetPosition(int position) { _flowGraphPosition = position;}
    int Position() { return _flowGraphPosition;}
    int FlowGraphNodePosition();

    // This is the interface to process node with current inputs and parameters
    virtual bool TryProcess() final;
    virtual void SetParameters(){};

protected:
    virtual bool BeforeProcessing();
    virtual bool AfterProcessing();

    // Default implementation copies _input to _ouptut
    virtual bool ProcessInternal();

public:
    //
    virtual QWidget * NodeUiBaseWidgetInForm() = 0;
    virtual QLayout* NodeUiBaseLayoutInForm() = 0;
    virtual QWidget * SpecificUI() = 0;

    // Peak widget is instantiated when needed and deleted when not visible
    virtual QWidget * InstantiatePeakWidget() = 0;
    virtual void UpdatePeakWidget() = 0;
    virtual void ReleasePeakWidget() = 0;

    // This could be generic
    // And in cpp file
    QImage * Input()
    {
        return _input;
    }

    QImage * Output()
    {
        return _output;
    }

    virtual void SetInput(QImage * input)
    {
        _input = input;
    }

    NodeCommonWidget * CommonWidget()
    {
        return _nodeCommonWidget;
    }

    // Is SUpposed to be called in Node implementation constructor
    virtual void InitializeNodeCommonWidget();

    bool IsEnabled() { return _isEnabled;}
    void Enable(bool isEnabled){ _isEnabled = isEnabled;}

    void OnEnableProcessorCheckboxClicked(bool toggled);
    void OnExpandCollapseArrowClicked(bool isSpecificWidgetExpanded);

signals:
    void OnDeleteProcessorClicked(Node * node);
    void NodeChanged(Node * node);
    void OnProcessorSettingsClicked();

protected :

    // FlowGraph is a list/line for now, _flowPosition is node index in this list
    int _flowGraphPosition;
    NodeType _nodeType;
    QString _name;
    bool _isEnabled;

    NodeCommonWidget * _nodeCommonWidget;

    QWidget * _peakWidget;

    // Type is dependant on implementation, should Node be a template ?
    //NodeSpecificUi * _specificUi;
    // Or SpecificUi() might suffice

    // This could be generic
    QImage * _input;
    QImage * _output;

    QString GetTempImageOutputFilePath();
    void EmitNodeChanged();
};

#endif // NODE_H
