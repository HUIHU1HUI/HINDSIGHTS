/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ED2V1AudioProcessorEditor::ED2V1AudioProcessorEditor (ED2V1AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (500, 600);
    setResizable(false, false);
    juce::HighResolutionTimer::startTimer(100);
    
    background = juce::ImageCache::getFromMemory(BinaryData::background_png, BinaryData::background_pngSize);
    
    
    ff_lnf.setColour(foleys::LevelMeter::lmMeterGradientLowColour, juce::Colours::grey);
    ff_lnf.setColour(foleys::LevelMeter::lmMeterGradientMidColour, juce::Colours::grey);
    ff_lnf.setColour(foleys::LevelMeter::lmMeterGradientMidColour, juce::Colours::ghostwhite);
    ff_lnf.setColour(foleys::LevelMeter::lmBackgroundColour, juce::Colours::transparentWhite);
    ff_lnf.setColour(foleys::LevelMeter::lmOutlineColour, juce::Colours::transparentWhite);
    ff_lnf.setColour(foleys::LevelMeter::lmMeterOutlineColour, juce::Colours::transparentWhite);
    ff_lnf.setColour(foleys::LevelMeter::lmMeterBackgroundColour, juce::Colours::transparentWhite);
    ff_lnf.setColour(foleys::LevelMeter::lmTicksColour, juce::Colours::transparentWhite);
    ff_lnf.setColour(foleys::LevelMeter::lmMeterGradientMaxColour, juce::Colours::grey);
    ff_lnf.setColour(foleys::LevelMeter::lmMeterMaxNormalColour, juce::Colours::transparentWhite);
    ff_lnf.setColour(foleys::LevelMeter::lmMeterMaxWarnColour, juce::Colours::transparentWhite);
    ff_lnf.setColour(foleys::LevelMeter::lmMeterMaxOverColour, juce::Colours::transparentWhite);
    ff_lnf.setColour(foleys::LevelMeter::lmBackgroundClipColour, juce::Colours::transparentWhite);

    ff_meter.setLookAndFeel(&ff_lnf);
    ff_meter.setMeterSource(&audioProcessor.getMeterSource());
    addAndMakeVisible(ff_meter);
    
    lowResBuffer.setSize(2, 100);
    
    /* Buffer 1 */
    RateSliderBuffer1.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    RateSliderBuffer1.setLookAndFeel(&EDLookAndFeel1);
    RateSliderBuffer1.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    RateSliderBuffer1Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"BUFFER1PLAYRATE", RateSliderBuffer1);
    addAndMakeVisible(RateSliderBuffer1);
    
    GainSliderBuffer1.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    GainSliderBuffer1.setLookAndFeel(&EDLookAndFeel1);
    GainSliderBuffer1.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    GainSliderBuffer1Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"BUFFER1GAIN", GainSliderBuffer1);
    addAndMakeVisible(GainSliderBuffer1);
    
    PanSliderBuffer1.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    PanSliderBuffer1.setLookAndFeel(&EDLookAndFeel1);
    PanSliderBuffer1.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    PanSliderBuffer1Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"BUFFER1PAN", PanSliderBuffer1);
    addAndMakeVisible(PanSliderBuffer1);
    
    LengthSliderBuffer1.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    LengthSliderBuffer1.setLookAndFeel(&EDLookAndFeel1);
    LengthSliderBuffer1.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    LengthSliderBuffer1Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"BUFFER1LENGTH", LengthSliderBuffer1);
    addAndMakeVisible(LengthSliderBuffer1);
    
    RepeatSliderBuffer1.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    RepeatSliderBuffer1.setLookAndFeel(&EDLookAndFeel1);
    RepeatSliderBuffer1.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    RepeatSliderBuffer1Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"BUFFER1REPEATS", RepeatSliderBuffer1);
    addAndMakeVisible(RepeatSliderBuffer1);
    
    /* Buffer 2 */
    RateSliderBuffer2.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    RateSliderBuffer2.setLookAndFeel(&EDLookAndFeel1);
    RateSliderBuffer2.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    RateSliderBuffer2Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"BUFFER2PLAYRATE", RateSliderBuffer2);
    addAndMakeVisible(RateSliderBuffer2);
    
    GainSliderBuffer2.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    GainSliderBuffer2.setLookAndFeel(&EDLookAndFeel1);
    GainSliderBuffer2.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    GainSliderBuffer2Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"BUFFER2GAIN", GainSliderBuffer2);
    addAndMakeVisible(GainSliderBuffer2);
    
    PanSliderBuffer2.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    PanSliderBuffer2.setLookAndFeel(&EDLookAndFeel1);
    PanSliderBuffer2.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    PanSliderBuffer2Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"BUFFER2PAN", PanSliderBuffer2);
    addAndMakeVisible(PanSliderBuffer2);
    
    LengthSliderBuffer2.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    LengthSliderBuffer2.setLookAndFeel(&EDLookAndFeel1);
    LengthSliderBuffer2.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    LengthSliderBuffer2Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"BUFFER2LENGTH", LengthSliderBuffer2);
    addAndMakeVisible(LengthSliderBuffer2);
    
    RepeatSliderBuffer2.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    RepeatSliderBuffer2.setLookAndFeel(&EDLookAndFeel1);
    RepeatSliderBuffer2.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    RepeatSliderBuffer2Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"BUFFER2REPEATS", RepeatSliderBuffer2);
    addAndMakeVisible(RepeatSliderBuffer2);
    
    /* Delay */
    
    DelayGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    DelayGainSlider.setLookAndFeel(&EDLookAndFeel1);
    DelayGainSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    DelayGainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"DELAYGAIN", DelayGainSlider);
    addAndMakeVisible(DelayGainSlider);
    
    DelayTimeSliderL.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    DelayTimeSliderL.setLookAndFeel(&EDLookAndFeel1);
    DelayTimeSliderL.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    DelayTimeSliderLAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"DELAYTIMEL", DelayTimeSliderL);
    addAndMakeVisible(DelayTimeSliderL);
    
    DelayTimeSliderR.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    DelayTimeSliderR.setLookAndFeel(&EDLookAndFeel1);
    DelayTimeSliderR.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    DelayTimeSliderRAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"DELAYTIMER", DelayTimeSliderR);
    addAndMakeVisible(DelayTimeSliderR);
    
    DelayFBSliderL.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    DelayFBSliderL.setLookAndFeel(&EDLookAndFeel1);
    DelayFBSliderL.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    DelayFBSliderLAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"DELAYFEEDBACKL", DelayFBSliderL);
    addAndMakeVisible(DelayFBSliderL);
    
    DelayFBSliderR.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    DelayFBSliderR.setLookAndFeel(&EDLookAndFeel1);
    DelayFBSliderR.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    DelayFBSLiderRAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"DELAYFEEDBACKR", DelayFBSliderR);
    addAndMakeVisible(DelayFBSliderR);
    
    //DRYWETs
    DryWetSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    DryWetSlider.setLookAndFeel(&EDLookAndFeel1);
    DryWetSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    DryWetSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"DRYWET", DryWetSlider);
    addAndMakeVisible(DryWetSlider);
    
}

ED2V1AudioProcessorEditor::~ED2V1AudioProcessorEditor()
{
    juce::HighResolutionTimer::stopTimer();
    ff_meter.setLookAndFeel(nullptr);
}

//==============================================================================


void ED2V1AudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::ghostwhite);
    g.setColour (juce::Colours::white);
    g.drawImage(background, 0, 0, 500, 600, 0, 0, 500, 600);
    g.setFont (15.0f);
    
    
    
}

void ED2V1AudioProcessorEditor::hiResTimerCallback()
{
    //NOT YET USED
}

void ED2V1AudioProcessorEditor::resized()
{
    int mainKnobDiam = 40;
    
    GainSliderBuffer1.setBounds(87, 190, mainKnobDiam, mainKnobDiam);
    LengthSliderBuffer1.setBounds(230, 190, mainKnobDiam, mainKnobDiam);
    RepeatSliderBuffer1.setBounds(315, 190, mainKnobDiam, mainKnobDiam);
    RateSliderBuffer1.setBounds(386, 190, mainKnobDiam, mainKnobDiam);
    
    GainSliderBuffer2.setBounds(87, 295, mainKnobDiam, mainKnobDiam);
    LengthSliderBuffer2.setBounds(230, 295, mainKnobDiam, mainKnobDiam);
    RepeatSliderBuffer2.setBounds(315, 295, mainKnobDiam, mainKnobDiam);
    RateSliderBuffer2.setBounds(386, 295, mainKnobDiam, mainKnobDiam);
    
    DelayTimeSliderL.setBounds(315, 400, mainKnobDiam, mainKnobDiam);
    DelayTimeSliderR.setBounds(386, 400, mainKnobDiam, mainKnobDiam);
    DelayFBSliderL.setBounds(315, 495, mainKnobDiam, mainKnobDiam);
    DelayFBSliderR.setBounds(386, 495, mainKnobDiam, mainKnobDiam);
    DelayGainSlider.setBounds(87, 400, mainKnobDiam, mainKnobDiam);
    
    DryWetSlider.setBounds(87, 495, mainKnobDiam, mainKnobDiam);
    
    ff_meter.setBounds((getWidth() / 2) - 20, 392, 40, 170);
    
    
}
