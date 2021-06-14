/*
  ==============================================================================

    EDBuffer.h
    Created: 15 Apr 2021 7:52:45pm
    Author:  Lucas Hui

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <algorithm>
#include "reSampler.h"

/*
 Class for all buffer repetition and resampling functions
 */

class EDBuffer
{
public:
    EDBuffer();
    ~EDBuffer();
  
    /* initialise buffer */
    void initialise(double _sampleRate, float bufferLengthSecs);
    
    /* set samples into buffer */
    void setNextSample(float _sample);
    
    /* get sample from buffer */
    float getNextSample(int _channel);
    
    /* advance the buffer readings */
    void incrementReadHead();
    
    /* parameters update */
    void updateNumRepeats(int _newNumRepeats);
    void updateBufferLength(float _newBufferLength);
    void updateSpeedRatio(float _newSpeedRatio);
    void panStereoOutput(float _pan);
    void setOutGain(float _gain);
    
    /* NOT USED YET */
    void resampleLowResWaveform();
    juce::Atomic<bool> GUIwaveformReady = false;
    juce::LinearInterpolator lowResInterpolatorL, lowResInterpolatorR;
    juce::AudioBuffer<float> lowResBuffer;
    
private:
    
    void incrementWriteHead();
    void changeBufferPlayLength();
    
    int bufferPlayLengthHolder;
    
    juce::AudioBuffer<float> audioBuffer;
    double samplerate;
    int bufferLength, bufferPlayLength;
    
    void multiplyWithWindow();
    
    juce::AudioBuffer<float> lowPassHoldingBuffer;
    void downSampleLowPass(double _targetSampleRate);
    
    float lin_InterpSamples(float _readHead, int _channel);
    float readHeadFloor, readHeadCeiling, inPhase;
    int audioBufferReadH;
    float audioBufferReadHFloat;
    float readHeadIncrement;
    int audioBufferWriteH;
    bool audioBufferRecordArmed;
    
    float sampleHoldOut;
    int repeats, repeatCounter;
    
    float panVol[2];
    float outGain;
    
    //SOON
    int LFOVal;
    float LFOAmount;
    
    juce::AudioBuffer<float> holdingBuffer;
    
    juce::LinearInterpolator interpolatorL;
    juce::LinearInterpolator interpolatorR;
    juce::AudioBuffer<float> resampledBuffer;
    juce::IIRFilter lowPass [2];
    juce::IIRCoefficients lowPassCoefficients;
    
    juce::dsp::WindowingFunction<float> window;
    
    float speedRatio = 1.0;
    void pushToResampleBuffer(bool _fromWH);
    juce::AudioBuffer<float> fullRecHold, fullResampledHold;
    int playLengthResampled;
    void incrementRepeats();
    
    
    void calculateNewReadPos();
    int numSamplesResampledBuffer = 0;
    int samplesLeftToPlay = 0;
    
    void resampleOnLoopEnd();
    float currentSpeedRatio;
    bool isFirstPlay = true;
    int currentResampledBufferSize;
    
    /* parameter update management */
    float lastGainSliderValue;
    int lastRepeatSliderValue;
    float lastLengthSliderValue;
    int lastRateSliderValue;
    float lastPanSliderValue;
};
