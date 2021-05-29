#include "ProcessorNode.h"

#include <QString>
#include <QDebug>
#include <QElapsedTimer>

IProcessorNode::IProcessorNode():
    _isEnabled(true)
{
}

bool IProcessorNode::TryProcess()
{
    qDebug() << "##################";

    // Read inputs (if changed ?)
    if( BeforeProcessing() == false)
    {
        qDebug() << "Failed to process " << __FUNCTION__;
        return false;
    }

    qDebug() << "Processing " << __FUNCTION__;

    // Display processing time
    QString processingLogOutput = "";
    QElapsedTimer processingTimer;
    processingTimer.start();
    int processingTime;

    ProcessInternal();

    processingTime = processingTimer.elapsed();
    processingLogOutput += "took " + QString::number(processingTime) + "ms to process." + (_isEnabled == false ? "(disabled)":"");

    if( false/*_output == nullptr*/)
    {
        qDebug() << "_ouput is null";
        return false;
    }

    //update();
    //CommonWidget()->OnPeakClicked();

    AfterProcessing();

    // Write outputs

    //CommonWidget()->ShowLastProcessingTime(processingTime);

    qDebug() << processingLogOutput;
    qDebug() << "----------------------";

    //emit NodeOutputChanged(this);

    return true;
}

bool IProcessorNode::BeforeProcessing()
{
    // Read inputs, initialize outputs

    //for( int i = 0; i < _imageFilterExamplePins.Count(); i++)
    {
        /*
        GenericNodePin * nodePin; = _imageFilterExamplePins[i];
        if( nodePin->IsInput())
        {
            QString pinName = nodePin->Name();
            if(pinName == "sortingAngle")
            {
                // do something with nodePin->GetData();
                //gliltchParam.SetSortingAngle(nodePin->GetData());
            }
            else if(pinName == "imageInput")
            {
                // do something with nodePin->GetData();
            }
            else
            {
                qDebug() << __FUNCTION__ << " this pin is not handled: " << pinName;
            }
        }
        */
    }

    return true;
}

bool IProcessorNode::AfterProcessing() const
{

    return true;
}
