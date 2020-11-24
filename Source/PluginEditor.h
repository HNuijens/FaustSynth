/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/


class SynthFaustPolyAudioProcessorEditor  : public juce::AudioProcessorEditor,
    private juce::MidiInputCallback,
    private juce::MidiKeyboardStateListener
{

   
public:
    SynthFaustPolyAudioProcessorEditor (SynthFaustPolyAudioProcessor&);
    ~SynthFaustPolyAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

private:
    AudioProcessorValueTreeState& valueTreeState;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    void handleNoteOn(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float /*velocity*/) override;
    void handleIncomingMidiMessage(MidiInput* source, const MidiMessage& message) override;


    MidiKeyboardState keyboardState;
    MidiKeyboardComponent keyboardComponent;

    Slider cutoffSlider;
    std::unique_ptr<SliderAttachment> cutoffAttachment;


    Slider gainSlider;
    std::unique_ptr<SliderAttachment> gainAttachment;

    juce::ToggleButton onOffButton1; //square
    juce::ToggleButton onOffButton2; //saw
    juce::ToggleButton onOffButton3; //tri
    juce::ToggleButton onOffButton4; //noise
    std::unique_ptr<ButtonAttachment> squareAttachment;
    std::unique_ptr<ButtonAttachment> sawAttachment;
    std::unique_ptr<ButtonAttachment> triAttachment;
    std::unique_ptr<ButtonAttachment> noiseAttachment;





    Label cutoffLabel;
    Label gainLabel;
   
    juce::Label onOffLabel1;
    juce::Label onOffLabel2;
    juce::Label onOffLabel3;
    juce::Label onOffLabel4;

    SynthFaustPolyAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthFaustPolyAudioProcessorEditor)
};
