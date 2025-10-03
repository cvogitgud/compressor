/*
  ==============================================================================

    LabeledSlider.cpp
    Created: 2 Oct 2025 11:29:41am
    Author:  Chris

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LabeledSlider.h"

//==============================================================================
LabeledSlider::LabeledSlider(const juce::String& paramId)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    addAndMakeVisible(slider);
    slider.setSliderStyle(juce::Slider::Rotary);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    
    addAndMakeVisible(label);
    label.setFont(juce::FontOptions(12.0f));
    label.setText(paramId, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);
}

LabeledSlider::~LabeledSlider()
{
}

void LabeledSlider::reset(juce::AudioProcessorValueTreeState& treeState, const juce::String& paramId){
    sliderAttachment.reset(new SliderAttachment(treeState, paramId, slider));
}

void LabeledSlider::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void LabeledSlider::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    
    int width = getWidth();
    int height = getHeight();
    
    slider.setBounds(0, 0, width, height * 0.75);
    label.setBounds(0, slider.getBottom(), width, height * 0.25);
}
