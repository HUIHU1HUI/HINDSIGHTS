/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "EDLookAndFeel.h"
#include "EDBuffer.h"
#include "EDTimer.h"
//==============================================================================
/**
*/
class ED2V1AudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::HighResolutionTimer
{
public:
    ED2V1AudioProcessorEditor (ED2V1AudioProcessor&);
    ~ED2V1AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    
    void hiResTimerCallback() override;
    
private:
    ED2V1AudioProcessor& audioProcessor;

    EDLookAndFeel1 EDLookAndFeel1;

    int lowResWaveformSize = 0;
    juce::AudioBuffer<float> lowResBuffer;
    
    juce::Image background;
    
    
    /* Sliders */
    
    /* Buffer 1 */

    juce::Slider GainSliderBuffer1;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> GainSliderBuffer1Attachment;
    
    juce::Slider PanSliderBuffer1;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> PanSliderBuffer1Attachment;
    
    juce::Slider LengthSliderBuffer1;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> LengthSliderBuffer1Attachment;
    
    juce::Slider RepeatSliderBuffer1;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> RepeatSliderBuffer1Attachment;
    
    juce::Slider RateSliderBuffer1;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> RateSliderBuffer1Attachment;
    
    
    /* Buffer 2 */
    
    juce::Slider GainSliderBuffer2;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> GainSliderBuffer2Attachment;
    
    juce::Slider PanSliderBuffer2;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> PanSliderBuffer2Attachment;
    
    juce::Slider LengthSliderBuffer2;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> LengthSliderBuffer2Attachment;
    
    juce::Slider RepeatSliderBuffer2;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> RepeatSliderBuffer2Attachment;
    
    juce::Slider RateSliderBuffer2;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> RateSliderBuffer2Attachment;

    /* Delay */
    
    juce::Slider DelayGainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> DelayGainSliderAttachment;
    
    juce::Slider DelayTimeSliderL;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> DelayTimeSliderLAttachment;
    
    juce::Slider DelayTimeSliderR;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> DelayTimeSliderRAttachment;
    
    juce::Slider DelayFBSliderL;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> DelayFBSliderLAttachment;
    
    juce::Slider DelayFBSliderR;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> DelayFBSLiderRAttachment;
    
    
    
    /* Dry Wet */
    
    juce::Slider DryWetSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> DryWetSliderAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ED2V1AudioProcessorEditor)
};
