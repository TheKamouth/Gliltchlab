#include "glitchsegmentationrule.h"

GlitchSegmentationRule::GlitchSegmentationRule() :
    currentRule(SegmentationRule::Threshold),
    _threshold(120),
    _segmentationEnabled(false)
{

}

GlitchSegmentationRule::GlitchSegmentationRule(bool isEnabled, int threshold) :
    currentRule(SegmentationRule::Threshold),
    _threshold(threshold),
    _segmentationEnabled(isEnabled)
{

}
