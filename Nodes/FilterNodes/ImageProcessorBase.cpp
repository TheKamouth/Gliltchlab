#include "ImageProcessorBase.h"

#include <QDateTime>

void ImageProcessorBase::Process()
{
    BeforeProcessing();

    ProcessInternal();

    AfterProcessing();
}

QString ImageProcessorBase::GetTempImageOutputFilePath()
{
    QString nodeName = Name();
    QString timeStamp = QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss_z");
    return "image_output" + timeStamp + ".png";
}
