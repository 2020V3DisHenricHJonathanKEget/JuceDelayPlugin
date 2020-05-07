/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CircularBuffer.h"

#define MAX_DELAY_TIME 2
#define LEFT_CHANNEL 0
#define RIGHT_CHANNEL 1

//==============================================================================
/**
*/
class DelayPluginAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    DelayPluginAudioProcessor();
    ~DelayPluginAudioProcessor();
    
    void addParameters();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    float linearInterpolate(float sampleX, float sampleX1, float inPhase);

private:
    // The host wants to know what process parameters are used in the plugin, that why we use AudioParameters
    AudioParameterFloat* _DryWetParameter;
    AudioParameterFloat* _FeedbackParameter;
    AudioParameterFloat* _DelayTimeParameter;
    
    CircularBuffer* _CircularBuffer = nullptr;
    
    float _DelayTimeInSamples;
    
    float _DelayTimeSmoothed;
    
    float _FeedbackLeft;
    float _FeedbackRight;
    
    double _SampleRate;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayPluginAudioProcessor)
};
