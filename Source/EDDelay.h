/*
  ==============================================================================

    EDDelay.h
    Created: 20 Apr 2021 12:46:45pm
    Author:  Lucas Hui

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "EDOVRDrivr.h"
//SIMPLE MONO DELAY WITH FRACTION DELAY LINE AND FEEDBACK

class EDDelay
{
public:
    EDDelay();
    ~EDDelay();
    
    /*initialise Buffer length and head positions */
    void initialise(double _sampleRate);
    
    /* push sample into buffer */
    void setSample(float _sample);
    
    /* get next sample and advance the delay */
    float getNextSample();
    
    /* set parameters */
    void setDelayTime (float _delayTimeFraction);
    void setFeedBackAmount (float _feedBackAmount);
    void setInGain (float _inGain);
    void setFilterFrequency(float _freq);
    
    EDOVRDrivr EDOVRDrivr;
    
private:
    
    double sampleRate;
    
    void incrementWriteHead();
    void incrementReadHead();
    
    juce::AudioBuffer<float> delayBuffer;
    float interpolateSamples(float _readHead);
    
    int writeHead;
    float readHead;
    juce::LinearSmoothedValue<float> readHeadSmoothed;
    
    int maxDelayTime;
    float delayTimeFraction;
    float lastDelayTimeFraction;
    float currentDelayTime;
    
    float feedBackVal;
    float feedBackAmt;
    
    float inGain;
    float lastInGain;
    
    float lastFeedBackAmount;

    float filterFrequency;
};
