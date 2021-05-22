#ifndef GLITCHPARAMETERS_H
#define GLITCHPARAMETERS_H

#include "glitchsortingrule.h"
#include "glitchsegmentationrule.h"
#include "glitchmask.h"

class GlitchParameters
{
public:
    void Init();

    GlitchParameters();

    float sortingAngle;
    bool isCircularSorting;
    QPointF sortingCenter;
    GlitchSortingRule* sortingRule;
    bool invertSortingRule;
    GlitchSegmentationRule* segmentationRule;

    // unused
    GlitchMask mask;

};

#endif // GLITCHPARAMETERS_H
