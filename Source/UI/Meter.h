/*
  ==============================================================================

    Meter.h
    Created: 10 Oct 2025 12:43:41pm
    Author:  Chris

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MeterBackground.h"
#include "MeterNeedle.h"

//==============================================================================
/*
*/
class Meter  : public juce::Component
{
public:
    Meter();
    ~Meter() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void update(const float& val);
    float getValue();

private:
    MeterBackground meterBG;
    MeterNeedle needle;
    
    float valueInDecibel;
    float startAngle, endAngle;
//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Meter)
};
