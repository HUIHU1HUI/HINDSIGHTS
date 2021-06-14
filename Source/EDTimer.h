/*
  ==============================================================================

    EDTimer.h
    Created: 20 Apr 2021 11:58:42am
    Author:  Lucas Hui

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <iostream>


class EDTimer : public juce::Timer
{
public:
    
    
    void timerCallback() override
    {
        std::cout<<"WORKING Callback"<<std::endl;
    }
    
private:
    
  
};
