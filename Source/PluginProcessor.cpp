/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"



//==============================================================================
ED2V1AudioProcessor::ED2V1AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
apvts(*this, nullptr, "parameters", createParameters())
#endif
{
}

ED2V1AudioProcessor::~ED2V1AudioProcessor()
{
}

//==============================================================================
const juce::String ED2V1AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ED2V1AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ED2V1AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ED2V1AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ED2V1AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ED2V1AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ED2V1AudioProcessor::getCurrentProgram()
{
    return 0;
}

void ED2V1AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ED2V1AudioProcessor::getProgramName (int index)
{
    return {};
}

void ED2V1AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ED2V1AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    
    buffer1[0].initialise(sampleRate, 3.0);
    buffer1[1].initialise(sampleRate, 3.0);
    
    buffer2[0].initialise(sampleRate, 3.0);
    buffer2[1].initialise(sampleRate, 3.0);
    
    Delay[0].initialise(sampleRate);
    Delay[1].initialise(sampleRate);
    
}

void ED2V1AudioProcessor::releaseResources()
{

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ED2V1AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void ED2V1AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    updateParameters();
    
    wetAmount = apvts.getRawParameterValue("DRYWET")->load();
    dryAmount = 1.0 - wetAmount;
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            
            buffer1[channel].setNextSample(*channelData);
            buffer2[channel].setNextSample(*channelData);
            float outSampleFromBuffers = (buffer1[channel].getNextSample(channel) + buffer2[channel].getNextSample(channel)) * 0.5;
            Delay[channel].setSample(outSampleFromBuffers);
            *channelData = (*channelData * dryAmount) + ((outSampleFromBuffers + Delay[channel].getNextSample()) * wetAmount);
            buffer1[channel].incrementReadHead();
            buffer2[channel].incrementReadHead();
            ++channelData;
            
        }
    }
}

void ED2V1AudioProcessor::updateParameters()
{
    buffer1[0].updateNumRepeats(apvts.getRawParameterValue("BUFFER1REPEATS")->load());
    buffer1[0].updateBufferLength(apvts.getRawParameterValue("BUFFER1LENGTH")->load());
    buffer1[0].panStereoOutput(apvts.getRawParameterValue("BUFFER1PAN")->load());
    buffer1[0].setOutGain(apvts.getRawParameterValue("BUFFER1GAIN")->load());
    buffer1[0].updateSpeedRatio(apvts.getRawParameterValue("BUFFER1PLAYRATE")->load());

    buffer1[1].updateNumRepeats(apvts.getRawParameterValue("BUFFER1REPEATS")->load());
    buffer1[1].updateBufferLength(apvts.getRawParameterValue("BUFFER1LENGTH")->load());
    buffer1[1].panStereoOutput(apvts.getRawParameterValue("BUFFER1PAN")->load());
    buffer1[1].setOutGain(apvts.getRawParameterValue("BUFFER1GAIN")->load());
    buffer1[1].updateSpeedRatio(apvts.getRawParameterValue("BUFFER1PLAYRATE")->load());
    
    buffer2[0].updateNumRepeats(apvts.getRawParameterValue("BUFFER2REPEATS")->load());
    buffer2[0].updateBufferLength(apvts.getRawParameterValue("BUFFER2LENGTH")->load());
    buffer2[0].panStereoOutput(apvts.getRawParameterValue("BUFFER2PAN")->load());
    buffer2[0].setOutGain(apvts.getRawParameterValue("BUFFER2GAIN")->load());
    buffer2[0].updateSpeedRatio(apvts.getRawParameterValue("BUFFER2PLAYRATE")->load());

    buffer2[1].updateNumRepeats(apvts.getRawParameterValue("BUFFER2REPEATS")->load());
    buffer2[1].updateBufferLength(apvts.getRawParameterValue("BUFFER2LENGTH")->load());
    buffer2[1].panStereoOutput(apvts.getRawParameterValue("BUFFER2PAN")->load());
    buffer2[1].setOutGain(apvts.getRawParameterValue("BUFFER2GAIN")->load());
    buffer2[1].updateSpeedRatio(apvts.getRawParameterValue("BUFFER2PLAYRATE")->load());

    Delay[0].setDelayTime(apvts.getRawParameterValue("DELAYTIMEL")->load());
    Delay[1].setDelayTime(apvts.getRawParameterValue("DELAYTIMER")->load());
    
    Delay[0].setInGain(apvts.getRawParameterValue("DELAYGAIN")->load());
    Delay[0].setFeedBackAmount(apvts.getRawParameterValue("DELAYFEEDBACKL")->load());
    Delay[1].setInGain(apvts.getRawParameterValue("DELAYGAIN")->load());
    Delay[1].setFeedBackAmount(apvts.getRawParameterValue("DELAYFEEDBACKR")->load());
    
    Delay[0].setFilterFrequency(apvts.getRawParameterValue("DELAYFILTERFREQ")->load());
    Delay[1].setFilterFrequency(apvts.getRawParameterValue("DELAYFILTERFREQ")->load());
}


//==============================================================================
bool ED2V1AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ED2V1AudioProcessor::createEditor()
{
    return new ED2V1AudioProcessorEditor (*this);
}

//==============================================================================
void ED2V1AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ED2V1AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ED2V1AudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout ED2V1AudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters;
    parameters.push_back(std::make_unique<juce::AudioParameterInt>("BUFFER1REPEATS", "Buffer 1 Repeats", 2, 20, 2));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("BUFFER1LENGTH", "Buffer 1 Length", 0.02, 0.6, 0.3));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("BUFFER1PLAYRATE", "Buffer 1 Rate", 0.5, 2.0, 1.0));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("BUFFER1PAN", "Buffer 1 Pan", -1.0, 1.0, -1.0));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("BUFFER1GAIN", "Buffer 1 Gain", 0.0, 1.0, 0.5));
    
    parameters.push_back(std::make_unique<juce::AudioParameterInt>("BUFFER2REPEATS", "Buffer 2 Repeats", 2, 20, 2));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("BUFFER2LENGTH", "Buffer 2 Length", 0.02, 0.6, 0.3));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("BUFFER2PLAYRATE", "Buffer 2 Rate", 0.5, 2.0, 1.0));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("BUFFER2PAN", "Buffer 2 Pan", -1.0, 1.0, 0.0));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("BUFFER2GAIN", "Buffer 2 Gain", 0.0, 1.0, 0.5));
    
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("DELAYTIMEL", "Left Delay Time", 0.01, 1.0, 0.3));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("DELAYFEEDBACKL", "Left Delay FeedBack", 0.0, 0.9, 0.4));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("DELAYGAIN", "Delay Gain", 0.0, 4.0, 1.5));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("DELAYTIMER", "Right Delay Time", 0.01, 1.0, 0.32));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("DELAYFEEDBACKR", "Right Delay FeedBack", 0.0, 0.9, 0.4));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("DELAYFILTERFREQ", "Delay HighPass Freq", 20.0, 10000.0, 20.0));
    
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("DRYWET", "Dry/Wet", 0.0, 1.0, 0.5));
    return {parameters.begin(), parameters.end()};
}
