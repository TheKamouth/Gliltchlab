#ifndef NODE_H
#define NODE_H

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
    virtual QString Name() { return "unnamed node"; }
    virtual QWidget * NodeUiBaseWidgetInForm() = 0;
    virtual QLayout* NodeUiBaseLayoutInForm() = 0;
    virtual QWidget * SpecificUI() = 0;

    // Peak widget is instantiated wjen needed and deleted when not visible
    virtual QWidget * InstantiatePeakWidget() = 0;

    // This could be generic
    QImage * Input() { return _input; }
    QImage * Output() { return _output; }

    NodeCommonWidget * CommonWidget() { return _nodeCommonWidget;}

    // Is SUpposed to be called in Node implementation constructor
    virtual void Initialize();

    bool IsEnabled() { return _isEnabled;}
    void Enable(bool isEnabled){ _isEnabled = isEnabled;}

    void OnEnableProcessorCheckboxClicked(bool toggled);
    void OnExpandCollapseArrowClicked(bool isSpecificWidgetExpanded);

signals:
    void OnDeleteProcessorClicked(Node * node);
    void OnProcessorSettingsClicked();

protected :
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
};

#endif // NODE_H
