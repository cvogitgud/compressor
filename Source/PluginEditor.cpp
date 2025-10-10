/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CompressorAudioProcessorEditor::CompressorAudioProcessorEditor (CompressorAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 300);

    addAndMakeVisible(thresholdSlider);
    thresholdSlider.reset(processor.treeState, paramThreshold);
    
    addAndMakeVisible(attackSlider);
    attackSlider.reset(processor.treeState, paramAttack);
    
    addAndMakeVisible(releaseSlider);
    releaseSlider.reset(processor.treeState, paramRelease);
    
    addAndMakeVisible(ratioChoices);
    auto choices = processor.treeState.getParameter(paramRatio)->getAllValueStrings();
    ratioChoices.addItemList(choices, 1);
    ratioAttachment.reset(new ComboBoxAttachment(processor.treeState, paramRatio, ratioChoices));
    
    addAndMakeVisible(outputSlider);
    outputSlider.reset(processor.treeState, paramOutput);
}

CompressorAudioProcessorEditor::~CompressorAudioProcessorEditor()
{
}

//==============================================================================
void CompressorAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
    
}

void CompressorAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto area = getLocalBounds().reduced(2.0f);
    float xMargin = 20.0f;
    
    auto colArea = juce::Rectangle(area.getWidth() / 6, area.getHeight());
    
    auto threshBoxArea = colArea.reduced(2);
    threshBoxArea.setPosition(xMargin, 0);
    
    auto attackReleaseBoxArea = colArea.reduced(2);
    attackReleaseBoxArea.setPosition(threshBoxArea.getRight() + xMargin, 0);
    
    auto outputBoxArea = colArea.reduced(2);
    outputBoxArea.setPosition(attackReleaseBoxArea.getRight() + xMargin, 0);
    
    area.removeFromLeft((colArea.getWidth() + xMargin) * 3);
    auto meterBoxArea = area.reduced(2.0f);
    meterBoxArea.setPosition(outputBoxArea.getRight() + xMargin, 0);

    // Threshold
    juce::FlexBox threshBox;
    threshBox.flexWrap = juce::FlexBox::Wrap::noWrap;
    threshBox.flexDirection = juce::FlexBox::Direction::column;
    threshBox.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    threshBox.items.add(juce::FlexItem(thresholdSlider).withFlex(1));
    threshBox.performLayout(threshBoxArea.toFloat());
    
    // Attack & Release
    juce::FlexBox attackReleaseBox;
    attackReleaseBox.flexWrap = juce::FlexBox::Wrap::noWrap;
    attackReleaseBox.flexDirection = juce::FlexBox::Direction::column;
    attackReleaseBox.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    attackReleaseBox.items.add(juce::FlexItem(attackSlider).withFlex(1));
    attackReleaseBox.items.add(juce::FlexItem(releaseSlider).withFlex(1));
    attackReleaseBox.performLayout(attackReleaseBoxArea.toFloat());
    
    // Right Output knob box
    juce::FlexBox outputBox;
    outputBox.flexWrap = juce::FlexBox::Wrap::noWrap;
    outputBox.flexDirection = juce::FlexBox::Direction::column;
    outputBox.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    outputBox.items.add(juce::FlexItem(outputSlider).withFlex(1));
    outputBox.performLayout(outputBoxArea.toFloat());
    
    // Meter Box
    juce::FlexBox meterBox;
    meterBox.flexWrap = juce::FlexBox::Wrap::noWrap;
    meterBox.flexDirection = juce::FlexBox::Direction::column;
    meterBox.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    
    // add in Meter component
    
    // ratio choices looks HUGE lmao
    meterBox.items.add(juce::FlexItem(ratioChoices).withFlex(1));
    meterBox.performLayout(meterBoxArea.toFloat());
    
}
