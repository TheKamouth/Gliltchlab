#include "ImageFilterExample.h"

ImageFilterExample::ImageFilterExample()
{

}

bool ImageFilterExample::BeforeProcessing()
{
    // Read all inputs in _imageFilterExamplePins and map it to parameters

    for (int i = 0; i < _imageFilterExamplePins.Count(); i++)
    {
        /*
        GenericNodePin * nodePin = _imageFilterExamplePins[i];
        if( nodePin->IsInput())
        {
            QString pinName = nodePin->Name();
            if(pinName == "imageInput")
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

bool ImageFilterExample::ProcessInternal()
{
    // Default interface implementation :
    // initialize outputs
    //_output = new QImage(/*default placholder output image*/);

    return true;
}

bool ImageFilterExample::AfterProcessing() const
{
    // Notify
    return true;
}
