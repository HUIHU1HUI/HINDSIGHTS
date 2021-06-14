/*
  ==============================================================================

    EDLookAndFeel.cpp
    Created: 19 Apr 2021 7:47:21pm
    Author:  Lucas Hui

  ==============================================================================
*/

#include "EDLookAndFeel.h"


EDLookAndFeel1::EDLookAndFeel1()
{
    knob1 = juce::ImageCache::getFromMemory(BinaryData::greyKnob1_png, BinaryData::greyKnob1_pngSize);
    
}
/* Interface with knobstrip PNG image file - Draws knob position when values are changed */
void EDLookAndFeel1::drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider &slider)
{
    if (knob1.isValid())
    {
        if (knob1.getWidth() > knob1.getHeight())
        {
            const double rotation = (slider.getValue() - slider.getMinimum()) / (slider.getMaximum() - slider.getMinimum());
            const int numFrames = knob1.getWidth() / knob1.getHeight();
            const int frameID = (int)ceil(rotation * ((double)numFrames - 1.0));
            
            
            g.drawImage(knob1, x, y, width, height, frameID * knob1.getHeight(), 0, knob1.getHeight(), knob1.getHeight());
        }
        if (knob1.getWidth() < knob1.getHeight())
        {
            const double rotation = (slider.getValue() - slider.getMinimum()) / (slider.getMaximum() - slider.getMinimum());
            const int numFrames = knob1.getHeight() / knob1.getWidth();
            const int frameID = (int)ceil(rotation * ((double)numFrames - 1.0));
            
            g.drawImage(knob1, x, y, width, height, 0, frameID * knob1.getWidth(), knob1.getWidth(), knob1.getWidth());
        }
    }
    else
    {
        std::cout<<"no image found"<<std::endl;
    }
}

