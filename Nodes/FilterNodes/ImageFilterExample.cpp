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
    for(int i = 0 ; i < PinCount::value; i++)
    {
        //qDebug() << "pin #" << i << ": " << GetPinName<i>();
    }
}

bool SimplerImageFilterNodeExample::ProcessInternal()
{
    /*GenericNodePin * pin = */
    //GetNodeGenericPinAt< SimplerImageFilterNodeExample,0 >(this);

    //IDataPin * pin0 = GetDataPinAt<0>();
    //FlowData * pin0Data = pin0->GetData();

    return true;
}
