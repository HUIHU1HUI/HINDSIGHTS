/*
  ==============================================================================

    EDBuffer.cpp
    Created: 15 Apr 2021 7:52:45pm
    Author:  Lucas Hui

  ==============================================================================
*/

#include "EDBuffer.h"


EDBuffer::EDBuffer() : window(1024, juce::dsp::WindowingFunction<float>::WindowingMethod::hann)
{
    
}
EDBuffer::~EDBuffer()
{
    
}
void EDBuffer::initialise(double _sampleRate, float bufferLengthSecs)
{
    samplerate = _sampleRate;
    bufferLength = 4 * samplerate;
    bufferPlayLength = bufferLength * 1.0;
    
    audioBuffer.setSize(1, bufferLength);
    audioBuffer.clear();
    holdingBuffer.setSize(1, bufferLength);
    holdingBuffer.clear();
    fullRecHold.setSize(1, bufferLength);
    fullRecHold.clear();
    fullResampledHold.setSize(1, 2 * bufferLength);
    fullResampledHold.clear();
    resampledBuffer.setSize(1, 2 * bufferLength);
    resampledBuffer.clear();
    
    audioBufferReadH = 0;
    readHeadIncrement = 1.0;
    audioBufferWriteH = 0;
    
    window.fillWindowingTables(bufferLength, juce::dsp::WindowingFunction<float>::hann);
    
    audioBufferRecordArmed = true;
    
    repeats = 5;
    repeatCounter = repeats;
    
    panVol[0] = 1.0;
    panVol[1] = 1.0;
        
    playLengthResampled = bufferPlayLength;
    
    lowPassCoefficients.makeLowPass(samplerate, samplerate / 2);
    lowPass[0].setCoefficients(lowPassCoefficients);
    lowPass[1].setCoefficients(lowPassCoefficients);
    
    currentResampledBufferSize = bufferLength;
}

void EDBuffer::setNextSample(float _sample)
{
    audioBuffer.setSample(0, audioBufferWriteH, _sample);
    incrementWriteHead();
}

float EDBuffer::getNextSample(int _channel)
{
    return (resampledBuffer.getSample(0, audioBufferReadH) * outGain); //(panVol[_channel] * outGain));
}

void EDBuffer::pushToResampleBuffer(bool _fromWH)
{
    currentSpeedRatio = speedRatio;
    numSamplesResampledBuffer = bufferLength * currentSpeedRatio;
    playLengthResampled = bufferPlayLength * currentSpeedRatio;
    
    if (_fromWH == true)
    {
        fullRecHold.copyFrom(0, 0, audioBuffer, 0, 0, bufferLength);
    }
    
    else
    {
        holdingBuffer.copyFrom(0, 0, fullRecHold, 0, 0, bufferLength);
        interpolatorL.process(currentSpeedRatio, fullRecHold.getReadPointer(0), resampledBuffer.getWritePointer(0), numSamplesResampledBuffer, bufferLength, 0);
        fullResampledHold.copyFrom(0, 0, resampledBuffer, 0, 0, numSamplesResampledBuffer);
        
        window.fillWindowingTables(playLengthResampled, juce::dsp::WindowingFunction<float>::hann);
        window.multiplyWithWindowingTable(resampledBuffer.getWritePointer(0), playLengthResampled);
        
        currentResampledBufferSize = numSamplesResampledBuffer;
        calculateNewReadPos();
     
        if (isFirstPlay == true)
        {
            isFirstPlay = false;
        }
    }
}

void EDBuffer::resampleOnLoopEnd()
{
    if (speedRatio != currentSpeedRatio)
    {
        currentSpeedRatio = speedRatio;
        numSamplesResampledBuffer = bufferLength * currentSpeedRatio;
        interpolatorL.process(speedRatio, fullRecHold.getReadPointer(0), resampledBuffer.getWritePointer(0), numSamplesResampledBuffer, bufferLength, 1);
        fullResampledHold.copyFrom(0, 0, resampledBuffer, 0, 0, numSamplesResampledBuffer);
        
        currentResampledBufferSize = numSamplesResampledBuffer;
        calculateNewReadPos();
        
        playLengthResampled = bufferPlayLength * currentSpeedRatio;
        
        window.fillWindowingTables(playLengthResampled, juce::dsp::WindowingFunction<float>::hann);
        window.multiplyWithWindowingTable(resampledBuffer.getWritePointer(0), playLengthResampled);
    }
}

void EDBuffer::multiplyWithWindow()
{
    for (int i = 0; i < resampledBuffer.getNumSamples(); ++i)
    {
        window.multiplyWithWindowingTable(resampledBuffer.getWritePointer(i), currentResampledBufferSize);
    }
}

void EDBuffer::calculateNewReadPos()
{
    if (audioBufferReadH >= currentResampledBufferSize)
    {
        audioBufferReadH = 0;
        incrementRepeats();
    }
}

void EDBuffer::incrementReadHead()
{
    ++audioBufferReadH;
    if (audioBufferReadH >= playLengthResampled || audioBufferReadH >= bufferLength || audioBufferReadH >= currentResampledBufferSize)
    {
        incrementRepeats();
        audioBufferReadH = 0;
    }
}

void EDBuffer::incrementWriteHead()
{
    ++audioBufferWriteH;
    if (audioBufferWriteH >= bufferLength)
    {
        pushToResampleBuffer(1);
        audioBufferWriteH = 0;
    }
}
void EDBuffer::incrementRepeats()
{
    --repeatCounter;
    if (repeatCounter == 0)
    {
        pushToResampleBuffer(0);
        repeatCounter = repeats;
                   
    }
    resampleOnLoopEnd();
    changeBufferPlayLength();
}

void EDBuffer::changeBufferPlayLength()
{
    bufferPlayLength = bufferPlayLengthHolder;
    playLengthResampled = bufferPlayLength * currentSpeedRatio;
    resampledBuffer.copyFrom(0, 0, fullResampledHold, 0, 0, playLengthResampled);
    window.fillWindowingTables(playLengthResampled, juce::dsp::WindowingFunction<float>::hann);
    window.multiplyWithWindowingTable(resampledBuffer.getWritePointer(0), playLengthResampled);
    
}

//update variables when sliders are changed
void EDBuffer::updateNumRepeats(int _newNumRepeats)
{
    if (_newNumRepeats != lastRepeatSliderValue)
    {
        repeats = _newNumRepeats;
        repeatCounter = repeats;
        lastRepeatSliderValue = _newNumRepeats;
    }
}

void EDBuffer::updateBufferLength(float _newBufferLength)
{
    if (_newBufferLength != lastLengthSliderValue)
    {
        bufferPlayLengthHolder = currentResampledBufferSize * _newBufferLength;
        lastLengthSliderValue = _newBufferLength;
    }
}

void EDBuffer::updateSpeedRatio(float _newSpeedRatio)
{
    if (_newSpeedRatio != lastRateSliderValue)
    {
        speedRatio = _newSpeedRatio;
        lastRateSliderValue = _newSpeedRatio;
    }
}

void EDBuffer::panStereoOutput(float _pan)
{
    if (_pan != lastPanSliderValue)
    {
        panVol[0] = juce::jmin(1.0 - _pan, 1.0);
        panVol[1] = juce::jmin(1.0 + _pan, 1.0);

        lastPanSliderValue = _pan;
    }
}

void EDBuffer::setOutGain(float _gain)
{
    if (_gain != lastGainSliderValue)
    {
        outGain = _gain;
        lastGainSliderValue = _gain;
    }
}

//NOT YET USED
void EDBuffer::resampleLowResWaveform()
{
    float lowResSize = samplerate / 100.0;
    lowResBuffer.setSize(1, lowResSize);
    lowResInterpolatorL.process(100, audioBuffer.getReadPointer(0), lowResBuffer.getWritePointer(0), lowResSize, bufferLength, 0);
    
    GUIwaveformReady = true;
    std::cout<<"is ready"<<" "<<GUIwaveformReady.get()<<std::endl;
}


