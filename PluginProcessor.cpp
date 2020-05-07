/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayPluginAudioProcessor::DelayPluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    addParameters();
    //_CircularBuffer = new CircularBuffer();
    _DelayTimeInSamples = 0;
    
    _FeedbackLeft = 0;
    _FeedbackRight = 0;
}

DelayPluginAudioProcessor::~DelayPluginAudioProcessor()
{
    if (_CircularBuffer != nullptr)
    {
        delete _CircularBuffer;
        _CircularBuffer = nullptr;
    }
}

//==============================================================================
const String DelayPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DelayPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DelayPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DelayPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DelayPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DelayPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DelayPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DelayPluginAudioProcessor::setCurrentProgram (int index)
{
}

const String DelayPluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void DelayPluginAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void DelayPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    _SampleRate = sampleRate;
    
    _DelayTimeInSamples = sampleRate * *_DelayTimeParameter;
    
    if (_CircularBuffer != nullptr)
    {
        delete _CircularBuffer;
        _CircularBuffer = nullptr;
    }
    
    _CircularBuffer = new CircularBuffer( sampleRate * MAX_DELAY_TIME );
}

void DelayPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DelayPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void DelayPluginAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    
    _DelayTimeInSamples = getSampleRate() * *_DelayTimeParameter;
    
    float* channelLeft = buffer.getWritePointer(0);
    float* channelRight = buffer.getWritePointer(1);
    
    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        _DelayTimeSmoothed = _DelayTimeSmoothed - 0.001 * (_DelayTimeSmoothed - *_DelayTimeParameter);
        _DelayTimeInSamples = _SampleRate * _DelayTimeSmoothed;
        
        _CircularBuffer->add(channelLeft[i] + _FeedbackLeft, channelRight[i] + _FeedbackLeft);
        
        _CircularBuffer->updateReadHead(_DelayTimeInSamples);
        
        int readHeadX = (int) _CircularBuffer->getReadHead();
        int readHeadX1 = readHeadX + 1;
        float readHeadFloat = _CircularBuffer->getReadHead() - readHeadX;
        
        if (readHeadX1 >= _CircularBuffer->getSize())
        {
            readHeadX1 -= _CircularBuffer->getSize();
        }
        
        float delaySampleLeft = linearInterpolate(_CircularBuffer->getSample(readHeadX, LEFT_CHANNEL), _CircularBuffer->getSample(readHeadX1, LEFT_CHANNEL), readHeadFloat);
        float delaySampleRight = linearInterpolate(_CircularBuffer->getSample(readHeadX, RIGHT_CHANNEL), _CircularBuffer->getSample(readHeadX1, RIGHT_CHANNEL), readHeadFloat);
        
        _FeedbackLeft = delaySampleLeft * *_FeedbackParameter;
        _FeedbackRight = delaySampleRight * *_FeedbackParameter;
        
        buffer.setSample(LEFT_CHANNEL, i, buffer.getSample(LEFT_CHANNEL, i) * (1 - *_DryWetParameter) + delaySampleLeft * *_DryWetParameter);
        buffer.setSample(RIGHT_CHANNEL, i, buffer.getSample(RIGHT_CHANNEL, i) * (1 - *_DryWetParameter) + delaySampleRight * *_DryWetParameter);
    }
}

//==============================================================================
bool DelayPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DelayPluginAudioProcessor::createEditor()
{
    return new DelayPluginAudioProcessorEditor (*this);
}

//==============================================================================
void DelayPluginAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DelayPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void DelayPluginAudioProcessor::addParameters()
{
    addParameter(_DryWetParameter = new AudioParameterFloat("drywet", "Dry/Wet", 0.0, 1.0, 0.5));
    addParameter(_FeedbackParameter = new AudioParameterFloat("feedback", "Feedback", 0.0, 1.0, 0.5));
    addParameter(_DelayTimeParameter = new AudioParameterFloat("delaytime", "Delay Time", 0.0, MAX_DELAY_TIME, 0.5));
}

float DelayPluginAudioProcessor::linearInterpolate(float sampleX, float sampleX1, float inPhase)
{
    //return (1 - inPhase) * sampleX + inPhase * sampleX1;
    return sampleX + inPhase * (sampleX1 - sampleX);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayPluginAudioProcessor();
}



