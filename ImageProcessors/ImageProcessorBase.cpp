#include "ImageProcessorBase.h"

void ImageProcessorBase::Process()
{
    BeforeProcessing();

    ProcessInternal();

    AfterProcessing();
}
