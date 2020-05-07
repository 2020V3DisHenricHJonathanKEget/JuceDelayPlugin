/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayPluginAudioProcessorEditor::DelayPluginAudioProcessorEditor (DelayPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (300, 130);
    
    auto& params = processor.getParameters();
    
    addControlSliders(params);
}

DelayPluginAudioProcessorEditor::~DelayPluginAudioProcessorEditor()
{
}

//==============================================================================
void DelayPluginAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void DelayPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void DelayPluginAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
    auto& params = processor.getParameters();
    
    if (slider == &_DryWetControlSlider)
    {
        AudioParameterFloat* dryWetParameter = (AudioParameterFloat*) params.getUnchecked(0);
        *dryWetParameter = _DryWetControlSlider.getValue();
    }
    else if (slider == &_FeedbackControlSlider)
    {
        AudioParameterFloat* feedbackParameter = (AudioParameterFloat*) params.getUnchecked(1);
        *feedbackParameter = _FeedbackControlSlider.getValue();
    }
    else if (slider == &_DelayTimeControlSlider)
    {
        AudioParameterFloat* delayTimeParameter = (AudioParameterFloat*) params.getUnchecked(2);
        *delayTimeParameter = _DelayTimeControlSlider.getValue();
    }
    else
    {
        DBG("WRONG PARAMETER-MAPPING");
    }
}

void DelayPluginAudioProcessorEditor::addControlSliders(Array<AudioProcessorParameter *> params)
{
    AudioParameterFloat* dryWetParameter = (AudioParameterFloat*) params.getUnchecked(0);
    AudioParameterFloat* feedbackParameter = (AudioParameterFloat*) params.getUnchecked(1);
    AudioParameterFloat* delayTimeParameter = (AudioParameterFloat*) params.getUnchecked(2);
    
    addDryWetSlider(dryWetParameter);
    addFeedbackSlider(feedbackParameter);
    addDelayTimeSlider(delayTimeParameter);
}

void DelayPluginAudioProcessorEditor::addDryWetSlider(AudioParameterFloat* dryWetParameter)
{
    _DryWetControlSlider.setBounds(0, 30, 100, 100);
    _DryWetControlSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    _DryWetControlSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 20);
    _DryWetControlSlider.setRange(dryWetParameter->range.start, dryWetParameter->range.end);
    _DryWetControlSlider.setValue(*dryWetParameter);
    _DryWetControlSlider.addListener(this);
    
    addAndMakeVisible(_DryWetControlSlider);
    
    addAndMakeVisible (_DryWetLabel);
    _DryWetLabel.setText ("Dry/Wet", dontSendNotification);
    _DryWetLabel.attachToComponent (&_DryWetControlSlider, false);
    
    /* This or .addListener(this)?
     _GainControlSlider.onValueChange = [this, gainParameter] { *gainParameter = _GainControlSlider.getValue(); };
     _GainControlSlider.onDragStart = [gainParameter] { gainParameter->beginChangeGesture(); };
     _GainControlSlider.onDragEnd = [gainParameter] { gainParameter->endChangeGesture(); };
     */
}

void DelayPluginAudioProcessorEditor::addFeedbackSlider(AudioParameterFloat* feedbackParameter)
{
    _FeedbackControlSlider.setBounds(100, 30, 100, 100);
    _FeedbackControlSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    _FeedbackControlSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 20);
    _FeedbackControlSlider.setRange(feedbackParameter->range.start, feedbackParameter->range.end);
    _FeedbackControlSlider.setValue(*feedbackParameter);
    _FeedbackControlSlider.addListener(this);
    
    addAndMakeVisible(_FeedbackControlSlider);
    
    addAndMakeVisible (_FeedbackLabel);
    _FeedbackLabel.setText ("Feedback", dontSendNotification);
    _FeedbackLabel.attachToComponent (&_FeedbackControlSlider, false);
}

void DelayPluginAudioProcessorEditor::addDelayTimeSlider(AudioParameterFloat* delayTimeParameter)
{
    _DelayTimeControlSlider.setBounds(200, 30, 100, 100);
    _DelayTimeControlSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    _DelayTimeControlSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 20);
    _DelayTimeControlSlider.setRange(delayTimeParameter->range.start, delayTimeParameter->range.end);
    _DelayTimeControlSlider.setValue(*delayTimeParameter);
    _DelayTimeControlSlider.addListener(this);
    
    addAndMakeVisible(_DelayTimeControlSlider);
    
    addAndMakeVisible (_DelayTimeLabel);
    _DelayTimeLabel.setText ("Delay Time", dontSendNotification);
    _DelayTimeLabel.attachToComponent (&_DelayTimeControlSlider, false);
}
