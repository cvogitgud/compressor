/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CompressorAudioProcessor::CompressorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), treeState(*this, nullptr, "PARAMS", createParameterLayout())
#endif
{
    treeState.addParameterListener(paramInput, this);
    treeState.addParameterListener(paramRatio, this);
    treeState.addParameterListener(paramThreshold, this);
    treeState.addParameterListener(paramAttack, this);
    treeState.addParameterListener(paramRelease, this);
    treeState.addParameterListener(paramOutput, this);
    treeState.addParameterListener(paramBypass, this);
}

CompressorAudioProcessor::~CompressorAudioProcessor()
{
    treeState.removeParameterListener(paramInput, this);
    treeState.removeParameterListener(paramRatio, this);
    treeState.removeParameterListener(paramThreshold, this);
    treeState.removeParameterListener(paramAttack, this);
    treeState.removeParameterListener(paramRelease, this);
    treeState.removeParameterListener(paramOutput, this);
    treeState.removeParameterListener(paramBypass, this);
}

//==============================================================================
const juce::String CompressorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool CompressorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool CompressorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool CompressorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double CompressorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int CompressorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int CompressorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void CompressorAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String CompressorAudioProcessor::getProgramName (int index)
{
    return {};
}

void CompressorAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void CompressorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    spec.maximumBlockSize = samplesPerBlock;
    
    inputGain.prepare(spec);
    outputGain.prepare(spec);
    envelopeFilter.prepare(spec);
    update();
}

void CompressorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CompressorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void CompressorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    juce::dsp::AudioBlock<float> block { buffer };
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        
        inputGain.process(juce::dsp::ProcessContextReplacing<float> (block));
        for (int sample = 0; sample < block.getNumSamples(); ++sample){
            channelData[sample] = processSample(channel, channelData[sample]);
        }
        outputGain.process(juce::dsp::ProcessContextReplacing<float> (block));

    }
}

float CompressorAudioProcessor::processSample(int channel, float inputValue){
    // Ballistics filter with peak rectifier
    auto env = envelopeFilter.processSample (channel, inputValue);

    // VCA
    auto gain = (env < threshold) ? static_cast<float> (1.0)
                                  : std::pow (env * thresholdInverse, ratioInverse - static_cast<float> (1.0));
    
    return gain * inputValue;
}

juce::AudioProcessorValueTreeState::ParameterLayout CompressorAudioProcessor::createParameterLayout(){
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    auto inputdB = std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("INPUT", 1), "Input", juce::NormalisableRange<float>(-10.0f, 10.0f), 0.0f);
    
    const juce::StringArray choices {"2:1", "4:1", "8:1", "20:1"};
    auto ratio = std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("RATIO", 1), "Ratio", choices, 0);
    
    auto thresholddB = std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("THRESHOLD", 1), "Threshold", juce::NormalisableRange<float>(-60.0f, 10.0f), 0.0f);
    
    // attack in MICROSECONDS
    auto attackTime = std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("ATTACK", 1), "Attack", juce::NormalisableRange<float>(20.0f, 800.0f, 0.5f), 400.0f);
    
    // release in MS
    auto releaseTime = std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("RELEASE", 1), "Release", juce::NormalisableRange<float>(50.0f, 1100.0f, 0.5f), 250.0f);
    
    auto bypass = std::make_unique<juce::AudioParameterBool>(juce::ParameterID("BYPASS", 1), "Bypass", false);
    
    auto outputdB = std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("OUTPUT", 1), "Output", juce::NormalisableRange<float>(-10.0f, 10.0f), 0.0f);
    
    params.push_back(std::move(inputdB));
    params.push_back(std::move(ratio));
    params.push_back(std::move(thresholddB));
    params.push_back(std::move(attackTime));
    params.push_back(std::move(releaseTime));
    params.push_back(std::move(bypass));
    params.push_back(std::move(outputdB));

    return { params.begin(), params.end() };
}

void CompressorAudioProcessor::parameterChanged(const juce::String& parameterId, float newValue) {
    // input -> input gain module
    if (parameterId == paramInput){
        inputGain.setGainDecibels(newValue);
    }
    // ratio -> choice -> int
    else if (parameterId == paramRatio){
        DBG("ratio choice index: " << newValue);
        switch (static_cast<int>(newValue)) {
            case RatioChoice::Two:
                ratio = 2;
                break;
            case RatioChoice::Four:
                ratio = 4;
                break;
            case RatioChoice::Eight:
                ratio = 8;
                break;
            case RatioChoice::Twenty:
                ratio = 20;
                break;
        }
        DBG("ratio: " << ratio);
    }
    else if (parameterId == paramThreshold){
        thresholddB = newValue;
    }
    else if (parameterId == paramAttack){
        attackTime = newValue;
    }
    else if (parameterId == paramRelease){
        releaseTime = newValue;
    }
    else if (parameterId == paramOutput){
        outputGain.setGainDecibels(newValue);
    }
    else if (parameterId == paramBypass){
        isBypassed = treeState.getRawParameterValue(paramBypass);
    }
    update();
}

void CompressorAudioProcessor::update(){
//    isBypassed = treeState.getRawParameterValue(paramBypass)->load();
    threshold = juce::Decibels::decibelsToGain(thresholddB, static_cast<float> (-200.0));
    thresholdInverse = static_cast<float> (1.0) / threshold;
    ratioInverse     = static_cast<float> (1.0) / ratio;

    envelopeFilter.setAttackTime (attackTime);
    envelopeFilter.setReleaseTime (releaseTime);
}


//==============================================================================
bool CompressorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* CompressorAudioProcessor::createEditor()
{
//    return new CompressorAudioProcessorEditor (*this);
    return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void CompressorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void CompressorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CompressorAudioProcessor();
}
