/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthFaustPolyAudioProcessorEditor::SynthFaustPolyAudioProcessorEditor(SynthFaustPolyAudioProcessor& p)
    : AudioProcessorEditor(&p), valueTreeState (valueTreeState) , processor(p), keyboardComponent(keyboardState, MidiKeyboardComponent::horizontalKeyboard)
{
    setSize(800, 210);
    addAndMakeVisible(keyboardComponent);
    keyboardState.addListener(this);


    addAndMakeVisible(cutoffSlider);
    cutoffSlider.setRange(50.0, 10000.0);
    cutoffSlider.setValue(5000.0);
    cutoffAttachment.reset(new SliderAttachment(valueTreeState, "cutoff", cutoffSlider));
    /*cutoffSlider.onValueChange = [this] {
        processor.setCutoff(cutoffSlider.getValue());
    };*/

    addAndMakeVisible(cutoffLabel);
    cutoffLabel.setText("Cutoff", dontSendNotification);
    cutoffLabel.attachToComponent(&cutoffSlider, true);

    ///============= Gain SLider ===========================//

    addAndMakeVisible(gainSlider);
    gainSlider.setRange(0.0, 1.0);
    gainSlider.setValue(0.5);
    gainAttachment.reset(new SliderAttachment(valueTreeState, "gain", gainSlider));
    /*gainSlider.onValueChange = [this] {
        processor.setGain(gainSlider.getValue());
    };*/

    addAndMakeVisible(gainLabel);
    gainLabel.setText("Gain", dontSendNotification);
    gainLabel.attachToComponent(&gainSlider, true);


    ///============= Square wave button ===========================//
    addAndMakeVisible(onOffButton1);
    squareAttachment.reset(new ButtonAttachment(valueTreeState, "square", onOffButton1));

    /*
    onOffButton1.onClick = [this] {
        processor.setGate1(onOffButton1.getToggleState());
    };
    */

    addAndMakeVisible(onOffLabel1);
    onOffLabel1.setText("Square", juce::dontSendNotification);
    onOffLabel1.attachToComponent(&onOffButton1, true);

    ///============= Saw wave button ===========================//
    addAndMakeVisible(onOffButton2);
    sawAttachment.reset(new ButtonAttachment(valueTreeState, "saw", onOffButton2));

    /*onOffButton2.onClick = [this] {
        processor.setGate2(onOffButton2.getToggleState());
    };*/

    addAndMakeVisible(onOffLabel2);
    onOffLabel2.setText("Saw", juce::dontSendNotification);
    onOffLabel2.attachToComponent(&onOffButton2, true);

    ///============= Tri wave button ===========================//
    addAndMakeVisible(onOffButton3);
    triAttachment.reset(new ButtonAttachment(valueTreeState, "tri", onOffButton3));
    /*
    onOffButton3.onClick = [this] {
        processor.setGate3(onOffButton3.getToggleState());
    };*/

    addAndMakeVisible(onOffLabel3);
    onOffLabel3.setText("Triangle", juce::dontSendNotification);
    onOffLabel3.attachToComponent(&onOffButton3, true);

    ///============= Noise wave button ===========================//
    addAndMakeVisible(onOffButton4);
    noiseAttachment.reset(new ButtonAttachment(valueTreeState, "noise", onOffButton4));
    /*
    onOffButton4.onClick = [this] {
        processor.setGate4(onOffButton4.getToggleState());
    };*/

    addAndMakeVisible(onOffLabel4);
    onOffLabel4.setText("Noise", juce::dontSendNotification);
    onOffLabel4.attachToComponent(&onOffButton4, true);
}

void SynthFaustPolyAudioProcessorEditor::handleIncomingMidiMessage(MidiInput* source, const MidiMessage& message) {}

void  SynthFaustPolyAudioProcessorEditor::handleNoteOn(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity)
{
    processor.keyOn(midiNoteNumber, int(127 * velocity));
}

void SynthFaustPolyAudioProcessorEditor::handleNoteOff(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float /*velocity*/)
{
    processor.keyOff(midiNoteNumber);
}

SynthFaustPolyAudioProcessorEditor::~SynthFaustPolyAudioProcessorEditor()
{
    keyboardState.removeListener(this);
}

//==============================================================================
void SynthFaustPolyAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void SynthFaustPolyAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    const int sliderLeft = 80;
    keyboardComponent.setBounds(10, 10, getWidth() - 30, 100);
    cutoffSlider.setBounds(sliderLeft, 120, getWidth() - sliderLeft - 20, 20);
    gainSlider.setBounds(sliderLeft,150, getWidth() - sliderLeft - 20, 20);
    onOffButton1.setBounds(sliderLeft, 180, getWidth() - sliderLeft - 20, 20);
    onOffButton2.setBounds(sliderLeft + 100, 180, getWidth() - sliderLeft - 20, 20);
    onOffButton3.setBounds(sliderLeft + 200, 180, getWidth() - sliderLeft - 20, 20);
    onOffButton4.setBounds(sliderLeft + 300, 180, getWidth() - sliderLeft - 20, 20);
}
