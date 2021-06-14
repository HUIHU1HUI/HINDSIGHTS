/*
  ==============================================================================
                
    EDDelay.cpp
    Created: 20 Apr 2021 12:46:45pm
    Author:  Lucas Hui

  ==============================================================================
*/

#include "EDDelay.h"

EDDelay::EDDelay()
{
}

EDDelay::~EDDelay()
{
    delayBuffer.clear();
}

void EDDelay::initialise(double _sampleRate)
{
    
    //Initialises a 5 second buffer, the longest gap possible between the heads being 4 seconds.
    sampleRate = _sampleRate;
    delayBuffer.setSize(1, 5 * sampleRate);
    delayBuffer.clear();
    writeHead = 0;
    maxDelayTime = 4 * sampleRate;
    readHead = delayBuffer.getNumSamples() / 2;
    
    //Initialise smoothed value
    readHeadSmoothed.reset(sampleRate, 0.5);
    readHeadSmoothed.setCurrentAndTargetValue(readHead);
    
    //Parameters
    inGain = 0.8;
    feedBackVal = 0.0;
    feedBackAmt = 0.0;
    filterFrequency = 20.0;
    
    EDOVRDrivr.initialise(filterFrequency, sampleRate);
    
}

void EDDelay::setSample(float _sample)
{
    delayBuffer.setSample(0, writeHead, (EDOVRDrivr.process(_sample * inGain)) + feedBackVal);
    incrementWriteHead();
}

void EDDelay::incrementWriteHead()
{
    ++writeHead;
    if (writeHead >= delayBuffer.getNumSamples())
    {
        writeHead = 0;
    }
}

void EDDelay::incrementReadHead()
{
    /*Don't advance the playhead by normal rate until the smoothed value has caught up.
     This allows for heavily modulating pitches when delay time value is changed */
    if (readHeadSmoothed.isSmoothing() == true)
    {
        readHead = readHeadSmoothed.getNextValue();
    }
    
    else
    {
        readHead += 1.0;
    }
    
    if (readHead >= delayBuffer.getNumSamples())
    {
        /* reset retaining fractional read */
        readHead -= std::floor(readHead);
    }
}

float EDDelay::getNextSample()
{
    float sampleOut = interpolateSamples(readHead);
    incrementReadHead();
    feedBackVal = sampleOut * feedBackAmt;
    return sampleOut;
}

float EDDelay::interpolateSamples(float _readHead)
{
    /* standard linear interpolation */
    
    auto X1 = (int) std::floor(_readHead);
    auto X2 = (X1 + 1) % delayBuffer.getNumSamples();
    float phase = _readHead - X1;
    
    return (1.0 - phase) * delayBuffer.getSample(0, X1) + phase * delayBuffer.getSample(0, X2);
}

void EDDelay::setDelayTime(float _delayTimeFraction)
{
    if (_delayTimeFraction != lastDelayTimeFraction)
    {
        /* if the value is different to the last known value (current value), ramp there */
        readHeadSmoothed.setTargetValue(delayBuffer.getNumSamples() - (maxDelayTime * _delayTimeFraction));
    }
    if (readHead == delayBuffer.getNumSamples() - (maxDelayTime * _delayTimeFraction))
    {
        lastDelayTimeFraction = _delayTimeFraction;
    }
    
}

void EDDelay::setFeedBackAmount(float _feedBackAmount)
{
    if (_feedBackAmount != lastFeedBackAmount)
    {
        feedBackAmt = _feedBackAmount;
        lastFeedBackAmount = _feedBackAmount;
    }
}

void EDDelay::setInGain(float _inGain)
{
    if (_inGain != lastInGain)
    {
        inGain = _inGain;
        lastInGain = _inGain;
    }
}

void EDDelay::setFilterFrequency(float _freq)
{
    EDOVRDrivr.setFilterFrequency(_freq);
}
