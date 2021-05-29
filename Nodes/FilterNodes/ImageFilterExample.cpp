#include "ImageFilterExample.h"

ImageFilterNodeExample::ImageFilterNodeExample()
{

}

bool ImageFilterNodeExample::BeforeProcessing()
{
    // Read all inputs in _imageFilterExamplePins and map it to parameters

    for( int i = 0; i < PinCount::value; i++)
    {

        //GenericNodePin * nodePin = nullptr; //_imageFilterExamplePins[i];
        /*if( nodePin->IsInput())
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
        }*/
    }

    return true;
}

bool ImageFilterNodeExample::ProcessInternal()
{
    // Default interface implementation :
    // initialize outputs
    //_output = new QImage(/*default placholder output image*/);

    return true;
}

bool ImageFilterNodeExample::AfterProcessing() const
{
    // Notify
    return true;
}

bool SimplerImageFilterNodeExample::BeforeProcessing()
{
    // Is a generic reading of all inputs possible ?
    //for(int i = 0 ; i < PinCount::value; i++)
    //{
        //qDebug() << "pin #" << i << ": " << SimplerImageFilterGenericNodeType::GetPinName<i>();
    //}

    IDataPin * pin0 = GetDataPinAt<0>();
    qDebug() << "pin #0: " << pin0->Name();
    //FlowData * pin0Data = pin0->GetData();

    IDataPin * pin1 = GetDataPinAt<1>();
    qDebug() << "pin #1: " << pin1->Name();
    //FlowData * pin1Data = pin1->GetData();

    IDataPin * pin2 = GetDataPinAt<2>();
    qDebug() << "pin #2: " << pin2->Name();
    //FlowData * pin2Data = pin2->GetData();

    return true;
}

bool SimplerImageFilterNodeExample::ProcessInternal()
{
    /*GenericNodePin * pin = */
    //GetNodeGenericPinAt< SimplerImageFilterNodeExample,0 >(this);

    IDataPin * pin0 = GetDataPinAt<0>();
    FlowData * pin0Data = pin0->GetData();

    return true;
}
