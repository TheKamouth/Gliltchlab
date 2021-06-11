#include "INode.h"

#include <QDebug>
#include <QElapsedTimer>

INode::INode() :
    _name("unnamed node"),
    _processedThisFrame(false),
    _lastFrameProcessingTimeMs(0.0f)
{}

QPointF INode::FlowGraphNodePosition()
{
    return _flowGraphNodePostion;
}

void INode::SetFlowGraphScenePosition(QPointF graphScenePosition)
{
    _flowGraphNodePostion = graphScenePosition;
}

int INode::GetPinVerticalIndex(IDataPin * pin)
{
    int inputIndex = 0;
    int outputIndex = 0;

    QList<IDataPin*> dataPinList = GetDataPinList();
    for( int i = 0 ; i < GetPinCount() ; i++)
    {
        IDataPin * pinAtIndex = dataPinList[i];
        if( pinAtIndex == pin)
        {
            return pin->IsInput() ? inputIndex : outputIndex;
        }

        if(pinAtIndex->IsInput())
        {
            inputIndex++;
        }
        else
        {
            outputIndex++;
        }
    }

    qDebug() << __FUNCTION__ << "Pin not found.";

    return -1;
}

bool INode::IsEnabled()
{
    return _isEnabled;
}

void INode::SetEnabled(bool enable)
{
    _isEnabled = enable;
}

bool INode::TryProcess()
{
    // Read inputs (if changed ?)
    // Does haveSourceNodesBeenProcessed suffices ?
    bool haveSourceNodesBeenProcessed = HaveSourceNodesBeenProcessed();

    if( haveSourceNodesBeenProcessed == false || BeforeProcessing() == false)
    {
        qDebug() << "Failed to process " << __FUNCTION__;
        return false;
    }

    // Display processing time
    QString processingLogOutput = "";
    QElapsedTimer processingTimer;
    processingTimer.start();

    ProcessInternal();

    _lastFrameProcessingTimeMs = processingTimer.elapsed();
    processingLogOutput += "took " + QString::number(_lastFrameProcessingTimeMs) + "ms to process." /*+ (_isEnabled == false ? "(disabled)":"")*/;

    if( false/*_output == nullptr*/)
    {
        qDebug() << "_ouput is null";
        return false;
    }

    //update();
    //CommonWidget()->OnPeakClicked();

    AfterProcessing();

    // Write outputs

    //emit NodeOutputChanged(this);

    _processedThisFrame = true;

    return true;
}

void INode::OnNewFrame()
{
    _processedThisFrame = false;
}

bool INode::HasBeenProcessedThisFrame()
{
    return _processedThisFrame;
}

float INode::LastFrameProcessingTime()
{
    return _lastFrameProcessingTimeMs;
}

float INode::MemoryConsumption()
{
    return 0;
}

bool INode::BeforeProcessing()
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

bool INode::AfterProcessing() const
{
    emit NodeOutputChanged(this);

    return true;
}
