#ifndef NODE_H
#define NODE_H

#include "NodeFactory.h"

#include <QString>
#include <QWidget>

class NodeCommonWidget;
class INodeConnection;

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
    virtual bool TryReadInputs();

protected:
    virtual bool BeforeProcessing();
    // Default implementation copies _input to _ouptut
    virtual bool ProcessInternal();
    // AfterProcessing should not modify _output
    virtual bool AfterProcessing();

public:
    //
    virtual QWidget * NodeUiBaseWidgetInForm() = 0;
    virtual QLayout * NodeUiBaseLayoutInForm() = 0;
    virtual QWidget * SpecificUI() = 0;
    virtual QLayout * SpecificUiLayout(){return nullptr;}

    // Peak widget is instantiated when needed and deleted when not visible
    // Not anymore: mainwindow handles 1 widget for each output type
    //virtual QWidget * InstantiatePeakWidget() = 0;
    //virtual void UpdatePeakWidget() = 0;
    //virtual void ReleasePeakWidget() = 0;

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

    // Common ui events handling
    void OnEnableNodeCheckboxClicked(bool toggled);
    void OnExpandCollapseArrowClicked(bool isSpecificWidgetExpanded);

    void EmitNodeInputChanged();

    template<class T>
    void EmitNodeInputChanged(T arg)
    {
        EmitNodeInputChanged();
    }

signals:
    void OnDeleteNodeClicked(Node * node);
    void NodeInputChanged(Node * node);
    void NodeOutputChanged(Node * node);
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

    // a cheap attempt for something a little bit more generic
    QVector< INodeConnection *> _flowData;

    QString GetTempImageOutputFilePath();

};

#endif // NODE_H
