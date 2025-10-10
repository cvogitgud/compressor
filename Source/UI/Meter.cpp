/*
  ==============================================================================

    Meter.cpp
    Created: 10 Oct 2025 12:43:41pm
    Author:  Chris

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Meter.h"

//==============================================================================
Meter::Meter()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    startAngle = static_cast<float>(5.0f / 3.0f * juce::MathConstants<float>::pi);
    endAngle = static_cast<float>(7.0f / 3.0f * juce::MathConstants<float>::pi);
//    meterBG.prepare(startAngle, endAngle);
//    needle.prepare(startAngle, endAngle);

    addAndMakeVisible(meterBG);
    addAndMakeVisible(needle);
}

Meter::~Meter()
{
}

void Meter::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (14.0f));
    g.drawText ("Meter", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void Meter::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    float width = getWidth();
    float height = getHeight();

}

void Meter::update(const float& val){
    if (val != valueInDecibel){
//        needle.update(val);
    }
}

float Meter::getValue(){
    return valueInDecibel;
}
