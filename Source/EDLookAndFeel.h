/*
  ==============================================================================

    EDLookAndFeel.h
    Created: 19 Apr 2021 7:47:21pm
    Author:  Lucas Hui

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

/*Classes for different images for sliders, overriding drawRotarySlider function*/

class EDLookAndFeel1 : public juce::LookAndFeel_V4
{
public:
    EDLookAndFeel1();
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                          float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;
  
private:
    juce::Image knob1;
    
};


