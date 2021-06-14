/*
  ==============================================================================

    reSampler.h
    Created: 17 Apr 2021 11:40:19am
    Author:  Lucas Hui

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class resampler
{
public:
    resampler();
    ~resampler();
    
    //LOW RES RESAMPLING FOR GUI GRAPHICS
    
    
    
    
    
private:
    juce::LinearInterpolator interpolatorL, interpolatorR; 
    
};
