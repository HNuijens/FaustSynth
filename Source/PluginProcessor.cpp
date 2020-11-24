/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthFaustPolyAudioProcessor::SynthFaustPolyAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
     ), parameters(*this, nullptr, Identifier("faust poly synth"), createParameterLayout())
#endif
{
    dspFaust.start();
}

SynthFaustPolyAudioProcessor::~SynthFaustPolyAudioProcessor()
{
    dspFaust.stop();
}

void SynthFaustPolyAudioProcessor::keyOn(int pitch, int velocity)
{
    dspFaust.keyOn(pitch, velocity);

}

void SynthFaustPolyAudioProcessor::keyOff(int pitch)
{
    dspFaust.keyOff(pitch);
}

void SynthFaustPolyAudioProcessor::setCutoff(float cutoff)
{
    dspFaust.setParamValue("/Polyphonic/Voices/synth/cutoff", cutoff);
}

void SynthFaustPolyAudioProcessor::setGain(float gainOut)
{
    dspFaust.setParamValue("/Polyphonic/Voices/synth/gainOut", gainOut);
}

void SynthFaustPolyAudioProcessor::setGate1(bool gate1)
{
    if (gate1) {
        dspFaust.setParamValue("/Polyphonic/Voices/synth/gate1", 1);
    }
    else {
        dspFaust.setParamValue("/Polyphonic/Voices/synth/gate1", 0);
    }
}

void SynthFaustPolyAudioProcessor::setGate2(bool gate2)
{
    if (gate2) {
        dspFaust.setParamValue("/Polyphonic/Voices/synth/gate2", 1);
    }
    else {
        dspFaust.setParamValue("/Polyphonic/Voices/synth/gate2", 0);
    }
}

void SynthFaustPolyAudioProcessor::setGate3(bool gate3)
{
    if (gate3) {
        dspFaust.setParamValue("/Polyphonic/Voices/synth/gate3", 1);
    }
    else {
        dspFaust.setParamValue("/Polyphonic/Voices/synth/gate3", 0);
    }
}

void SynthFaustPolyAudioProcessor::setGate4(bool gate4)
{
    if (gate4) {
        dspFaust.setParamValue("/Polyphonic/Voices/synth/gate4", 1);
    }
    else {
        dspFaust.setParamValue("/Polyphonic/Voices/synth/gate4", 0);
    }
}

//==============================================================================
const juce::String SynthFaustPolyAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SynthFaustPolyAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SynthFaustPolyAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SynthFaustPolyAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SynthFaustPolyAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SynthFaustPolyAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SynthFaustPolyAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SynthFaustPolyAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SynthFaustPolyAudioProcessor::getProgramName (int index)
{
    return {};
}

void SynthFaustPolyAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SynthFaustPolyAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{

}

void SynthFaustPolyAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SynthFaustPolyAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
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

void SynthFaustPolyAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    //empty
    // call methods 

    for (auto messageReference : midiMessages)
    {
        auto message = messageReference.getMessage();
        if (message.isNoteOn())
        {
            keyOn(message.getNoteNumber(), message.getVelocity());
        }

        else if (message.isNoteOff())
        {
            keyOff(message.getNoteNumber());
        }

        
    }
}

//==============================================================================
bool SynthFaustPolyAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SynthFaustPolyAudioProcessor::createEditor()
{
    return new SynthFaustPolyAudioProcessorEditor (*this);
}

//==============================================================================
void SynthFaustPolyAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void SynthFaustPolyAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

AudioProcessorValueTreeState::ParameterLayout SynthFaustPolyAudioProcessor::createParameterLayout()
{
    AudioProcessorValueTreeState::ParameterLayout params;

    params.add (std::make_unique<AudioParameterFloat>("cutoff", "cutoff", 0.0f, 1.0f, 0.2f));
    params.add (std::make_unique<AudioParameterFloat>("gain", "gain", 0.0f, 1.0f, 0.5f));
    params.add (std::make_unique<AudioParameterBool>("square", "square", false));
    params.add (std::make_unique<AudioParameterBool>("saw", "Saw", false));
    params.add (std::make_unique<AudioParameterBool>("tri", "Tri", false));
    params.add (std::make_unique<AudioParameterBool>("noise", "Noise", false));

    return { params };
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SynthFaustPolyAudioProcessor();
}
