/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class DelayPluginAudioProcessorEditor  : public AudioProcessorEditor, public Slider::Listener
{
public:
    DelayPluginAudioProcessorEditor (DelayPluginAudioProcessor&);
    ~DelayPluginAudioProcessorEditor();
    
    void addControlSliders(Array<AudioProcessorParameter *> params);
    void addDryWetSlider(AudioParameterFloat* dryWetParameter);
    void addFeedbackSlider(AudioParameterFloat* feedbackParameter);
    void addDelayTimeSlider(AudioParameterFloat* delayTimeParameter);

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    void sliderValueChanged (Slider* slider) override;

private:
    Slider _FeedbackControlSlider;
    Slider _DelayTimeControlSlider;
    Slider _DryWetControlSlider;
    
    Label _FeedbackLabel;
    Label _DelayTimeLabel;
    Label _DryWetLabel;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DelayPluginAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayPluginAudioProcessorEditor)
};
