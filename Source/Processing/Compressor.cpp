/*
  ==============================================================================

    Compressor.cpp
    Created: 23 Sep 2025 4:20:38pm
    Author:  Chris

  ==============================================================================
*/

#include "Compressor.h"

template <typename SampleType>
Compressor<SampleType>::Compressor(){
    update();
}

//==============================================================================
/** Sets the threshold of the compressor in decibels */
template <typename SampleType>
void Compressor<SampleType>::setThreshold(SampleType newThreshold){
    thresholddB = newThreshold;
    update();
}

/** Sets the ratio of the compressor **/
template <typename SampleType>
void Compressor<SampleType>::setRatio(SampleType newRatio){
    ratio = newRatio;
    update();
}

/** Sets the attack time of the compressor in microseconds **/
template <typename SampleType>
void Compressor<SampleType>::setAttack(SampleType newAttack){
    attackTime = newAttack;
    update();
}

/** Sets the release time of the compressor in milliseconds **/
template <typename SampleType>
void Compressor<SampleType>::setRelease(SampleType newRelease){
    releaseTime = newRelease;
    update();
}

//==============================================================================
template <typename SampleType>
void Compressor<SampleType>::prepare(const juce::dsp::ProcessSpec& spec){
    jassert (spec.sampleRate > 0);
    jassert (spec.numChannels > 0);

    sampleRate = spec.sampleRate;

    envelopeFilter.prepare (spec);

    update();
    reset();
}

template <typename SampleType>
void Compressor<SampleType>::reset(){
    envelopeFilter.reset();
}

//==============================================================================
template <typename SampleType>
SampleType Compressor<SampleType>::processSample(int channel, SampleType inputValue){
    // Ballistics filter with peak rectifier
    auto env = envelopeFilter.processSample (channel, inputValue);

    // VCA
    auto gain = (env < threshold) ? static_cast<float> (1.0)
                                  : std::pow (env * thresholdInverse, ratioInverse - static_cast<float> (1.0));
    
    return gain * inputValue;
}

//==============================================================================
template <typename SampleType>
void Compressor<SampleType>::update(){
    threshold = juce::Decibels::decibelsToGain(thresholddB, static_cast<float> (-200.0));
    thresholdInverse = static_cast<float> (1.0) / threshold;
    ratioInverse     = static_cast<float> (1.0) / ratio;
    
    envelopeFilter.setAttackTime (attackTime);
    envelopeFilter.setReleaseTime (releaseTime);
}

//==============================================================================
template class Compressor<float>;
template class Compressor<double>;
