/*
  ==============================================================================

    MeterBackground.cpp
    Created: 10 Oct 2025 12:44:31pm
    Author:  Chris

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MeterBackground.h"

//==============================================================================
MeterBackground::MeterBackground()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    startAngle = 0.0f;
    endAngle = 0.0f;
    minVal = 0;
    maxVal = 0;
    step = 5;
    
    indicatorColour = juce::Colours::whitesmoke;
    setBufferedToImage(true);
}

MeterBackground::~MeterBackground()
{
}

void MeterBackground::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (14.0f));
    g.drawText ("MeterBackground", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
    
    auto centreX = 0;
    auto centreY = 0;
    auto length = 0;
    
    drawIndicators(g, centreX, centreY, length);
}

void MeterBackground::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void MeterBackground::drawIndicators(juce::Graphics &g, float centreX, float centreY, float length){
    
    const int indices = (abs(maxVal - minVal) / step) + 1;
    int valIter = minVal;
    
    // draw Indicators, from minVal to maxVal, from startAngle to endAngle
    for (int i = 0; i < indices; i++){
        auto mappedVal = juce::jmap(static_cast<float>(valIter), static_cast<float>(minVal), static_cast<float>(maxVal), startAngle, endAngle);
        
        // if mappedVal is out of range of circle, wrap
        if (mappedVal > juce::MathConstants<float>::twoPi){
            mappedVal -= juce::MathConstants<float>::twoPi;
        }
        
        // "start" of indicator, on the edge of the circle
        const float x2 = centreX + sin(mappedVal) * length;
        const float y2 = centreY - cos(mappedVal) * length;
        
        // radius length back to centre of circle
        const float rX = centreX - x2;
        const float rY = centreY - y2;
        const float rLength = sqrt(juce::square(rX) + juce::square(rY));
        
        // sine and cosine angles
        const float nX = rX / rLength;
        const float nY = rY / rLength;
        
        // where does this 7 come from? is this the length of the indicator?
        // sine and cosine multiplied by 7 to get...something? length of indicator i think
        const float xCof = nX * 7;
        const float yCof = nY * 7;
        
        // draw indicator
        g.drawArrow({x2, y2, x2 - xCof, y2 - yCof}, 2.0f, 0, 0);
        
        // draw next indicator for next value
        valIter += step;
    }
}
