#ifndef INODE_H
#define INODE_H

#include "NodeFactory.h"
#include "FlowGraph/FlowData.h"
#include "FlowGraph/DataPin.h"

#include <QString>
#include <QObject>

class INode : public QObject
{
    Q_OBJECT
public:
    INode():_name("unnamed node"){;}
    virtual ~INode(){}

    template<int i>
    static QString GetPinName(int index);

    virtual QString Name() {return _name; };
    virtual void SetName( QString name){ _name = name; };
    virtual NodeType Type() = 0;
    virtual FlowData * MainOutput() = 0;
    // is there a better way ?
    virtual int GetPinCount() = 0;

    virtual IDataPin * GetDataPinAt(int index) = 0;

    virtual int GetMaxInOutPin() = 0;
    QPointF FlowGraphNodePosition();
    void SetFlowGraphScenePosition(QPointF graphScenePosition);

    bool IsEnabled();
    void SetEnabled(bool enable);
    bool TryProcess();

signals:
    void OnDeleteNodeClicked(INode * node);
    void NodeInputChanged(INode * node);
    void NodeOutputChanged(const INode * node) const;
    void OnProcessorSettingsClicked();

protected:
    virtual bool BeforeProcessing();
    // Default implementation copies _input to _ouptut
    virtual bool ProcessInternal() = 0;
    bool AfterProcessing() const;

private:
    bool _isEnabled;
    QString _name;
    QPointF _flowGraphNodePostion;
};

#endif // INODE_H
