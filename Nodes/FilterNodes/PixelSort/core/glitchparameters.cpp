#include "glitchparameters.h"

GlitchParameters::GlitchParameters()
{
    Init();
}

void GlitchParameters::Init(){

    sortingAngle = 0.0;
    sortingRule = new GlitchSortingRule();
    segmentationRule = new GlitchSegmentationRule();
    isCircularSorting = false;
    invertSortingRule = false;
}
