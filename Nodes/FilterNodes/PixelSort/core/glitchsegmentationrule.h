#ifndef GLITCHSEGMENTATIONRULE_H
#define GLITCHSEGMENTATIONRULE_H

enum SegmentationRule{
    Threshold,
    Random,
    Half,
    Linear
};

class GlitchSegmentationRule
{
public:
    GlitchSegmentationRule();
    GlitchSegmentationRule(bool isEnabled, int threshold);

    SegmentationRule currentRule;

    int GetThreshold(){ return _threshold;}
    bool IsSegmentationEnabled(){ return _segmentationEnabled;}

private:
    int _threshold;
    bool _segmentationEnabled;
};

#endif // GLITCHSEGMENTATIONRULE_H
