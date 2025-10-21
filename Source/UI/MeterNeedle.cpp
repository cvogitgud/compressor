/*
  ==============================================================================

    MeterNeedle.cpp
    Created: 10 Oct 2025 12:44:40pm
    Author:  Chris

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MeterNeedle.h"

//==============================================================================
MeterNeedle::MeterNeedle()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    startAngle = 0.0;
    endAngle = 0.0;
    minVal = 0;
    maxVal = 0;
    step = 5;
    valueInDecibel = 0.0f;
}

MeterNeedle::~MeterNeedle()
{
}

void MeterNeedle::prepare(const float startAngle, const float endAngle){
    this->startAngle = startAngle;
    this->endAngle = endAngle;
    minVal = -30;
    needleColour = juce::Colours::silver;
}

void MeterNeedle::update(const float &val){
    if (val != valueInDecibel){
        valueInDecibel = val;
    }
}

void MeterNeedle::paint (juce::Graphics& g)
{

    const auto bounds = area.toFloat();
    const float centreX = bounds.getX() + bounds.getWidth() * 0.5f;
    const float centreY = bounds.getY() + bounds.getHeight();
    const float needleLength = juce::jmin(bounds.getWidth() * 0.75f, bounds.getHeight() * 0.75f);

    g.setColour (needleColour);
    redrawNeedle(g, centreX, centreY, needleLength);
}

void MeterNeedle::resized()
{
    area = getLocalBounds().reduced(3);
    repaint();
}

void MeterNeedle::redrawNeedle(juce::Graphics &g, float centreX, float centreY, float length){
    auto clampedVal = std::clamp(valueInDecibel, static_cast<float>(minVal), static_cast<float>(maxVal));
    auto mapped = juce::jmap(clampedVal, static_cast<float>(minVal), static_cast<float>(maxVal), startAngle, endAngle);
    
    // convert value to angle, and wrap if needed
    // if over twoPi, subtract itself by twoPi to get back in range
    // else don't subtract at all
    mapped -= mapped > juce::MathConstants<float>::twoPi ? juce::MathConstants<float>::twoPi : 0.0f;
    
    // calculate horizontal and vertical lengths of needle position based on angle
    const float x2 = centreX + sin(mapped) * length;
    const float y2 = centreY - cos(mapped) * length;
    g.drawArrow({centreX, centreY, x2, y2}, 2.0f, 0, 0);
}
