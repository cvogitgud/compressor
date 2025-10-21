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
    
    void prepare(const float startAngle, const float endAngle);
    void paint (juce::Graphics&) override;
    void resized() override;
    void update(const float& val);
    void redrawNeedle(juce::Graphics& g, float centreX, float centreY, float length);

private:
    juce::Rectangle<int> area;
    juce::Colour needleColour;
    
    float valueInDecibel;
    int minVal, maxVal;
    float startAngle, endAngle;
    int step;
    
//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MeterNeedle)
};
