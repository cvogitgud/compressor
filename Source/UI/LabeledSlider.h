/*
  ==============================================================================

    LabeledSlider.h
    Created: 2 Oct 2025 11:29:41am
    Author:  Chris

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

//==============================================================================
/*
*/
class LabeledSlider  : public juce::Component
{
public:
    LabeledSlider(const juce::String& paramId);
    ~LabeledSlider() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void reset(juce::AudioProcessorValueTreeState& treeState, const juce::String& paramId);
private:
    
    juce::Slider slider;
    std::unique_ptr<SliderAttachment> sliderAttachment;
    
    juce::Label label;
    
    
//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LabeledSlider)
};
