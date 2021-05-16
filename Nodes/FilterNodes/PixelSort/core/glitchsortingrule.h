#ifndef GLITCHSORTINGRULE_H
#define GLITCHSORTINGRULE_H

#include <QColor>

enum SortingRule{
    RedOnly,
    GreenOnly,
    BlueOnly,
    RGBMean,
    HueOnly,
    SaturationOnly,
    ValueOnly
};

//The "sorting rule" is the function f:color->value, not literraly a sorting rule
class GlitchSortingRule
{

public:
    GlitchSortingRule();

    SortingRule currentRule;

    int GetColorValue(QColor color);

    void SetSortingRule( SortingRule sortingRule);
};

#endif // GLITCHSORTINGRULE_H
