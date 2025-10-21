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
    
    void prepare(const float startAngle, const float endAngle);
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void drawIndicators(juce::Graphics& g, float centreX, float centreY, float length);

private:
    
    // minVal and maxVal are linked to startAngle and endAngle
    // they are used to map out angles to draw the indicators
    // starting from minVal/startAngle to maxVal/endAngle
    juce::Colour bgColour;
    juce::Colour indicatorColour;
    
    juce::Rectangle<int> meterArea;
    
    int minVal, maxVal;
    float startAngle, endAngle;
    int step;

//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MeterBackground)
};
