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
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
	)
#endif
{
	for (int i = 1; i < 5; ++i)
	{
		String name = "gate";
		name += i;
		auto gate = new AudioParameterBool(name, name, false);
		gate->addListener(this);
		addParameter(gate);
	}

	auto cutoff = new AudioParameterFloat("Cutoff", "Cutoff", 50.0f, 10000.0f, 5000.0f);
	cutoff->addListener(this);
	addParameter(cutoff);

	auto gain = new AudioParameterFloat("Gain", "Gain", 0.0f, 1.0f, 0.5f);
	gain->addListener(this);
	addParameter(gain);


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
	dspFaust.setParamValue("/Polyphonic/Voices/untitled/cutoff", cutoff);
}

void SynthFaustPolyAudioProcessor::setGain(float gainOut)
{
	dspFaust.setParamValue("/Polyphonic/Voices/untitled/gainOut", gainOut);
}

void SynthFaustPolyAudioProcessor::setGate1(bool gate1)
{
	if (gate1) {
		dspFaust.setParamValue("/Polyphonic/Voices/untitled/gate1", 1);
	}
	else {
		dspFaust.setParamValue("/Polyphonic/Voices/untitled/gate1", 0);
	}
}

void SynthFaustPolyAudioProcessor::setGate2(bool gate2)
{
	if (gate2) {
		dspFaust.setParamValue("/Polyphonic/Voices/untitled/gate2", 1);
	}
	else {
		dspFaust.setParamValue("/Polyphonic/Voices/untitled/gate2", 0);
	}
}

void SynthFaustPolyAudioProcessor::setGate3(bool gate3)
{
	if (gate3) {
		dspFaust.setParamValue("/Polyphonic/Voices/untitled/gate3", 1);
	}
	else {
		dspFaust.setParamValue("/Polyphonic/Voices/untitled/gate3", 0);
	}
}

void SynthFaustPolyAudioProcessor::setGate4(bool gate4)
{
	if (gate4) {
		dspFaust.setParamValue("/Polyphonic/Voices/untitled/gate4", 1);
	}
	else {
		dspFaust.setParamValue("/Polyphonic/Voices/untitled/gate4", 0);
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

void SynthFaustPolyAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String SynthFaustPolyAudioProcessor::getProgramName(int index)
{
	return {};
}

void SynthFaustPolyAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void SynthFaustPolyAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{

}

void SynthFaustPolyAudioProcessor::releaseResources()
{

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SynthFaustPolyAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
	juce::ignoreUnused(layouts);
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

void SynthFaustPolyAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{

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
	return new SynthFaustPolyAudioProcessorEditor(*this);
}

//==============================================================================
void SynthFaustPolyAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
}

void SynthFaustPolyAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
}

/// <summary>
/// Call all the faust setter methods here.
/// </summary>
/// <param name="parameterIndex">The index of the parameter.</param>
/// <param name="newValue">The normalised value of the parameter.</param>
void SynthFaustPolyAudioProcessor::parameterValueChanged(int parameterIndex, float newValue)
{
	switch (parameterIndex)
	{
	case 0:
		setGate1(newValue);
		break;
	case 1:
		setGate2(newValue);
		break;
	case 2:
		setGate3(newValue);
		break;
	case 3:
		setGate4(newValue);
		break;
	case 4:
		// cutoff
		setCutoff(getParameters()[parameterIndex]->getCurrentValueAsText().getFloatValue());
		break;
	case 5:
		// gain
		setGain(getParameters()[parameterIndex]->getCurrentValueAsText().getFloatValue());
		break;
	default:
		break;
	}
}

void SynthFaustPolyAudioProcessor::parameterGestureChanged(int parameterIndex, bool gestureIsStarting)
{
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new SynthFaustPolyAudioProcessor();
}
