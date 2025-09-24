/*
  ==============================================================================

    Compressor.h
    Created: 23 Sep 2025 4:20:38pm
    Author:  Chris
 
    Note: This is all modeled after JUCE's own Compressor module. I'm really
          only changing the DSP for the compression algorithm, and some other
          minor changes.
  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

template <typename SampleType>
class Compressor {
public:
    //==============================================================================
    Compressor();
    
    //==============================================================================
    /** Sets the threshold of the compressor in decibels */
    void setThreshold (SampleType newThreshold);
    
    /** Sets the ratio of the compressor **/
    void setRatio (SampleType newRatio);
    
    /** Sets the attack time of the compressor in microseconds **/
    void setAttack (SampleType newAttack);
    
    /** Sets the release time of the compressor in milliseconds **/
    void setRelease (SampleType newRelease);
    
    //==============================================================================
    /** Initializes the compressor */
    void prepare (const juce::dsp::ProcessSpec& spec);
    
    void reset ();
    
    //==============================================================================
    template <typename ProcessContext>
    void process (const ProcessContext& context) {
        const auto& inputBlock = context.getInputBlock();
        auto& outputBlock      = context.getOutputBlock();
        const auto numChannels = outputBlock.getNumChannels();
        const auto numSamples  = outputBlock.getNumSamples();

        jassert (inputBlock.getNumChannels() == numChannels);
        jassert (inputBlock.getNumSamples()  == numSamples);

        if (context.isBypassed)
        {
            outputBlock.copyFrom (inputBlock);
            return;
        }

        for (size_t channel = 0; channel < numChannels; ++channel)
        {
            auto* inputSamples  = inputBlock .getChannelPointer (channel);
            auto* outputSamples = outputBlock.getChannelPointer (channel);

            for (size_t i = 0; i < numSamples; ++i)
                outputSamples[i] = processSample ((int) channel, inputSamples[i]);
        }
    }
    
    SampleType processSample (int channel, SampleType inputValue);
private:
    //==============================================================================
    void update ();
    
    //==============================================================================
    juce::dsp::BallisticsFilter<float> envelopeFilter;
    
    float threshold, thresholdInverse, ratioInverse;
    
    double sampleRate = 44100.0;
    int ratio = 1.0f;
    
    float thresholddB = 0.0f, attackTime = 400.0f, releaseTime = 250.0f;
};
