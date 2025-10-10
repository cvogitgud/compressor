/*
  ==============================================================================

    MeterBackground.h
    Created: 10 Oct 2025 12:44:31pm
    Author:  Chris

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class MeterBackground  : public juce::Component
{
public:
    MeterBackground();
    ~MeterBackground() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MeterBackground)
};
