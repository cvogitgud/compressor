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
    
    addAndMakeVisible(inputSlider);
    inputSlider.reset(processor.treeState, paramInput);
    
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
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
    
}

void CompressorAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    int width = getWidth();
    int height = getHeight();
    int xMargin = 20;
    
    int colWidth = width / 6;
    inputSlider.setBounds(xMargin, 0, colWidth, height);
    
    ratioChoices.setBounds(inputSlider.getRight() + xMargin, 0, colWidth, 20);
    
}
