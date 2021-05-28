#ifndef PROCESSORNODE_H
#define PROCESSORNODE_H

class ProcessorNode
{
public:
    ProcessorNode();
    virtual bool TryProcess() final;

protected:
    virtual bool BeforeProcessing();
    // Default implementation copies _input to _ouptut
    virtual bool ProcessInternal() = 0;
    virtual bool AfterProcessing() const;

    // ProcessInternal implementation needs a way to acces _isEnabled (add accessor method?)
    bool _isEnabled;
};

#endif // PROCESSORNODE_H
