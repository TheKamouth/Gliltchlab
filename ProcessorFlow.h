#ifndef PROCESSORFLOW_H
#define PROCESSORFLOW_H

#include "Nodes/FilterNodes/ImageProcessorBase.h"

#include <QList>

class ProcessorFlow
{
public:
    ProcessorFlow();

    void PlayFlow();

    void AddProcessor(ImageProcessorBase* processor);
    void InsertProcessor(ImageProcessorBase* processor, int index);
    void RemoveProcessor(int index);

    void SetInput(QImage * input);
    QImage* Output();

private:

    // List of all processor in the flow, applied first to last
    QList<ImageProcessorBase*> _processors;
};

#endif // PROCESSORFLOW_H
