/*
  ==============================================================================

    MeterNeedle.h
    Created: 10 Oct 2025 12:44:40pm
    Author:  Chris

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class MeterNeedle  : public juce::Component
{
public:
    MeterNeedle();
    ~MeterNeedle() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MeterNeedle)
};
