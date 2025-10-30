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
    setSize (800, 300);

    addAndMakeVisible(thresholdSlider);
    thresholdSlider.reset(processor.treeState, paramThreshold);
    
    addAndMakeVisible(attackSlider);
    attackSlider.reset(processor.treeState, paramAttack);
    
    addAndMakeVisible(releaseSlider);
    releaseSlider.reset(processor.treeState, paramRelease);
    
    addAndMakeVisible(outputSlider);
    outputSlider.reset(processor.treeState, paramOutput);
    
    addAndMakeVisible(meter);
    
    addAndMakeVisible(ratioChoices);
    auto choices = processor.treeState.getParameter(paramRatio)->getAllValueStrings();
    ratioChoices.addItemList(choices, 1);
    ratioAttachment.reset(new ComboBoxAttachment(processor.treeState, paramRatio, ratioChoices));
}

CompressorAudioProcessorEditor::~CompressorAudioProcessorEditor()
{
}

//==============================================================================
void CompressorAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
    
}

void CompressorAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(2.0f);
    float xMargin = 10;
    auto margin = juce::FlexItem::Margin(0, xMargin, 0, xMargin);
    
    auto colWidth = area.getWidth() / 6;
    
    auto threshBoxArea = area.removeFromLeft(colWidth).reduced(2);
    auto outputBoxArea = area.removeFromLeft(colWidth).reduced(2);
    auto attackReleaseBoxArea = area.removeFromLeft(colWidth).reduced(2);
    auto meterBoxArea = area.reduced(2.0f);

    // Threshold
    juce::FlexBox threshBox;
    threshBox.flexWrap = juce::FlexBox::Wrap::noWrap;
    threshBox.flexDirection = juce::FlexBox::Direction::column;
    threshBox.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    threshBox.items.add(juce::FlexItem(thresholdSlider).withFlex(1).withMargin(margin));
    threshBox.performLayout(threshBoxArea.toFloat());
    
    // Output box
    juce::FlexBox outputBox;
    outputBox.flexWrap = juce::FlexBox::Wrap::noWrap;
    outputBox.flexDirection = juce::FlexBox::Direction::column;
    outputBox.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    outputBox.items.add(juce::FlexItem(outputSlider).withFlex(1).withMargin(margin));
    outputBox.performLayout(outputBoxArea.toFloat());
    
    // Attack & Release
    juce::FlexBox attackReleaseBox;
    attackReleaseBox.flexWrap = juce::FlexBox::Wrap::noWrap;
    attackReleaseBox.flexDirection = juce::FlexBox::Direction::column;
    attackReleaseBox.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    attackReleaseBox.items.add(juce::FlexItem(attackSlider).withFlex(1).withMargin(margin));
    attackReleaseBox.items.add(juce::FlexItem(releaseSlider).withFlex(1).withMargin(margin));
    attackReleaseBox.performLayout(attackReleaseBoxArea.toFloat());
    
    // Meter Box
    juce::FlexBox meterBox;
    meterBox.flexWrap = juce::FlexBox::Wrap::noWrap;
    meterBox.flexDirection = juce::FlexBox::Direction::column;
    meterBox.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    auto meterFlexItem = juce::FlexItem(meter).withHeight(area.getHeight() * 5 / 6);
    meterBox.items.add(meterFlexItem.withFlex(1).withMargin(margin));
    
    auto ratioFlexItem = juce::FlexItem(ratioChoices).withHeight(area.getHeight() / 6);
    meterBox.items.add(ratioFlexItem.withFlex(1).withMargin(margin));
    meterBox.performLayout(meterBoxArea.withTrimmedBottom(20.0f).toFloat());
}
