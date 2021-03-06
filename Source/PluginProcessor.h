/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "EDBuffer.h"
#include "EDDelay.h"
//==============================================================================
/**
*/
class ED2V1AudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    ED2V1AudioProcessor();
    ~ED2V1AudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void updateParameters();
    
    EDBuffer buffer1[2];
    EDBuffer buffer2[2];
    EDDelay Delay[2];
    
    juce::AudioProcessorValueTreeState apvts;
    
    
    
    foleys::LevelMeterSource& getMeterSource()
    {
        return ff_meterSource;
    }
    
    
private:
    
    float dryAmount, wetAmount;
    
    foleys::LevelMeterSource ff_meterSource;
    
    
    
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ED2V1AudioProcessor)
};
