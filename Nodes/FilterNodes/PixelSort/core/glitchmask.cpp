#include "glitchmask.h"

GlitchMask::GlitchMask() :
    maskType(NoMask)
{

}

void GlitchMask::SetMask(QImage* ptrMask, MaskType newMaskType){

    mask = ptrMask;

    maskType = newMaskType;
}

QSize GlitchMask::GetSize(){

    return mask->size();
}
