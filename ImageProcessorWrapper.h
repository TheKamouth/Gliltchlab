hj#ifndef IMAGEPROCESSORWRAPPER_H
#define IMAGEPROCESSORWRAPPER_H

// An image processor is :
//  - the actual image processor, a way to produce an image (from an input or from parameters)
//  - parameters
//  - ui to modify parameters and input


template <class Parameters, class ParametersUI>
class ImageProcessor
{
public:
    Parameters * GetParameters();
    ParametersUI * GetParametersUI();

private:
    Parameters * _parameters;
    ParametersUI * _parametersUI;
}
#endif // IMAGEPROCESSORWRAPPER_H
