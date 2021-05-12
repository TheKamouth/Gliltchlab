#include "ProcessorFlow.h"

ProcessorFlow::ProcessorFlow()
{

}

void ProcessorFlow::PlayFlow()
{
    for( int i = 0 ; i < _processors.count() ; i++ )
    {
        _processors[i]->Process();
    }
}

void ProcessorFlow::AddProcessor(ImageProcessorBase *processor)
{
    _processors.append(processor);
}

void ProcessorFlow::InsertProcessor(ImageProcessorBase *processor, int index)
{
    _processors.insert(index, processor);
}

void ProcessorFlow::RemoveProcessor(int index)
{
    _processors.removeAt(index);
}

void ProcessorFlow::SetInput(QImage *input)
{
    _processors[0]->SetInput(input);
}

QImage *ProcessorFlow::Output()
{
    return _processors[0]->Output();
    //return _output;
}
