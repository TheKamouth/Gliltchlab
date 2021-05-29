#ifndef PROCESSORNODE_H
#define PROCESSORNODE_H

class IProcessorNode
{
public:
    IProcessorNode();
    virtual bool TryProcess() final;

protected:
    virtual bool BeforeProcessing();
    // Default implementation copies _input to _ouptut
    virtual bool ProcessInternal() = 0;
    virtual bool AfterProcessing() const;
};

#endif // PROCESSORNODE_H
