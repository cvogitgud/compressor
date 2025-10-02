/*
  ==============================================================================

    LabeledSlider.h
    Created: 2 Oct 2025 11:29:41am
    Author:  Chris

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class LabeledSlider  : public juce::Component
{
public:
    LabeledSlider();
    ~LabeledSlider() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LabeledSlider)
};
