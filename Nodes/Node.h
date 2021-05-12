#ifndef NODE_H
#define NODE_H

#include "ImagePeak.h"

#include <QString>
#include <QWidget>

class NodeCommonWidget;

class Node : public QWidget, public ImagePeak
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
    virtual QWidget * GetPeakWidget() = 0;

    NodeCommonWidget * CommonWidget() { return _nodeCommonWidget;}

    // Is SUpposed to be called in Node implementation constructor
    void Initialize();

    bool IsEnabled() { return _isEnabled;}
    void Enable(bool isEnabled){ _isEnabled = isEnabled;}

    void OnEnableProcessorCheckboxClicked(bool toggled);
    void OnExpandCollapseArrowClicked(bool isSpecificWidgetExpanded);

signals:
    void OnDeleteProcessorClicked(Node * node);
    void OnProcessorSettingsClicked();

private :
    QString _name;
    bool _isEnabled;

    NodeCommonWidget * _nodeCommonWidget;

    // Type is dependant on implementation, should Node be a template ?
    //NodeSpecificUi * _specificUi;
    // Or SpecificUi() might suffice
};

#endif // NODE_H
