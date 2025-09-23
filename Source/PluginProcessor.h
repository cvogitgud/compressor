/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

enum RatioChoice { Two, Four, Eight, Twenty };

//==============================================================================
/**
*/
class CompressorAudioProcessor  : public juce::AudioProcessor,
                                  public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    CompressorAudioProcessor();
    ~CompressorAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    juce::AudioProcessorValueTreeState treeState;
    
    juce::String paramInput { "INPUT" };
    juce::String paramRatio { "RATIO" };
    juce::String paramThreshold { "THRESHOLD" };
    juce::String paramAttack { "ATTACK" };
    juce::String paramRelease { "RELEASE" };
    juce::String paramOutput { "OUTPUT" };
    juce::String paramBypass { "BYPASS" };

private:
    juce::dsp::Gain<float> inputGain, outputGain;
    juce::dsp::BallisticsFilter<float> envelopeFilter;
    
    double sampleRate = 44100.0;
    float threshold, thresholdInverse, ratioInverse;
    int ratio = 1.0f;
    float thresholddB = 0.0f, attackTime = 400.0f, releaseTime = 250.0f;
    bool isBypassed = false;

    //==============================================================================
    float processSample(int channel, float inputValue);
    
    //==============================================================================
    void update();
    
    //==============================================================================
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void parameterChanged(const juce::String& parameterId, float newValue) override;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CompressorAudioProcessor)
};
