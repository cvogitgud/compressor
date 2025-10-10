/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/LabeledSlider.h"
#include "UI/Meter.h"
#include "Shared/ParamIds.h"

using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;

//==============================================================================
/**
*/
class CompressorAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    CompressorAudioProcessorEditor (CompressorAudioProcessor&);
    ~CompressorAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    CompressorAudioProcessor& processor;
    
    LabeledSlider inputSlider { paramInput };
    LabeledSlider thresholdSlider { paramThreshold };
    LabeledSlider attackSlider { paramAttack };
    LabeledSlider releaseSlider { paramRelease };
    LabeledSlider outputSlider { paramOutput };
    
    juce::ComboBox ratioChoices;
    std::unique_ptr<ComboBoxAttachment> ratioAttachment;
    juce::Label ratioLabel;
    
    Meter meter;
    
    juce::ToggleButton bypass;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CompressorAudioProcessorEditor)
};
