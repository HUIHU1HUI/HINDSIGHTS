/*
  ==============================================================================

    EDOVRDrivr.h
    Created: 21 Apr 2021 10:58:00am
    Author:  Lucas Hui

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

//SIMPLE OVERDRIVE

class EDOVRDrivr
{
public:
    /* Initialise OVRDrivr and pre-filter */
    void initialise(float _freq, double _sampleRate)
    {
        sampleRate = _sampleRate;
        hiPassFilter.setCoefficients(filtercoefficients.makeHighPass(sampleRate, _freq));
        smoothedFilterFreq.reset(_sampleRate, 2.0);
        smoothedFilterFreq.setCurrentAndTargetValue(_freq);
        
    }
    /* Set frequency of Pre-Drive filter*/
    void setFilterFrequency(float _newFreq)
    {
        if (_newFreq != lastFilterFreq)
        {
            smoothedFilterFreq.setTargetValue(_newFreq);
            filtercoefficients.makeHighPass(sampleRate, smoothedFilterFreq.getNextValue());
            lastFilterFreq = _newFreq;
        }
    }
    /* Process a sample */
    float process(float _sample)
    {
        outSample = hiPassFilter.processSingleSampleRaw(_sample);
        return (2 / juce::MathConstants<float>::pi) * std::atan(outSample);
    };
    
private:
    juce::IIRFilter hiPassFilter;
    juce::IIRCoefficients filtercoefficients;

    float sampleRate;
    float outSample;
    
    float lastFilterFreq;
    float frequency;
    juce::LinearSmoothedValue<float> smoothedFilterFreq;
};
