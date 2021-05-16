#ifndef GLITCHMASK_H
#define GLITCHMASK_H

#include <QImage>

enum MaskType{
    NoMask,
    SegmentationMap,
    IgnoreMaskedAreasFillColor,
    IgnoreMaskedAreasFillInput,
    IgnoreMaskedAreasBlendBoundary,
    MaskCount
};

class GlitchMask
{
public:
    GlitchMask();

    void SetMask( QImage*, MaskType);

    QSize GetSize();

    QImage* GetMask(){ return mask;}

    MaskType GetMaskType(){ return maskType;}

private:
    QImage* mask;

    MaskType maskType;

};

#endif // GLITCHMASK_H
