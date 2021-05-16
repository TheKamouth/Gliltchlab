#include "glitchsortingrule.h"

GlitchSortingRule::GlitchSortingRule()
{
    currentRule = RedOnly;
}

int GlitchSortingRule::GetColorValue(QColor color){   

    switch( currentRule){

    case( RedOnly) :
        return color.red();

    case( GreenOnly) :
        return color.green();        

    case( BlueOnly) :
        return color.blue();        

    case( RGBMean) :
        return int(( color.red() + color.green() + color.blue()) / 3.0);

    case( HueOnly) :
        return color.toHsv().hue();

    case( SaturationOnly) :
        return color.toHsv().saturation();

    case( ValueOnly) :
        return color.toHsv().value();

    }
}

void GlitchSortingRule::SetSortingRule( SortingRule sortingRule){

    currentRule = sortingRule;
}
