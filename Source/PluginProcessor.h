/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DspFaust.h";

//==============================================================================
/**
*/
class SynthFaustPolyAudioProcessor : public juce::AudioProcessor,
	AudioParameterBool::Listener
{
public:
	void keyOn(int pitch, int  velocity);
	void keyOff(int pitch);
	void setCutoff(float cutoff);
	void setGain(float gainOut);
	void setGate1(bool gate1); // control the square wave
	void setGate2(bool gate2); //control the saw wave
	void setGate3(bool gate3); //control the triangle wave
	void setGate4(bool gate4); //control the noise wave

	//==============================================================================
	SynthFaustPolyAudioProcessor();
	~SynthFaustPolyAudioProcessor() override;

	//==============================================================================
	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
	bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

	void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

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
	void setCurrentProgram(int index) override;
	const juce::String getProgramName(int index) override;
	void changeProgramName(int index, const juce::String& newName) override;

	//==============================================================================
	void getStateInformation(juce::MemoryBlock& destData) override;
	void setStateInformation(const void* data, int sizeInBytes) override;

private:

	DspFaust dspFaust;

	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthFaustPolyAudioProcessor)

	void parameterValueChanged(int parameterIndex, float newValue) override;
	void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override;
};
