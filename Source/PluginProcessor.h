/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class SimpleEQAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    SimpleEQAudioProcessor();
    ~SimpleEQAudioProcessor() override;

    //==============================================================================
    // prepareToPlay and processBlock are the two main functions that you need to implement in your plugin

    // gets called by the host when it's about to start playback
    // For pre-playback setup/stuff
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif
    // What actually happens to the audio when you hit the play button in the transport controller
    // The host starts sending buffers at a regular rate into your plugin then it's your plugins job to send back any finished audio that it is done processing
    // If it has latency/intterupts that buffer sending and recieving between the host and the plugin you'll hear clicks and pop into the plugin
    // This is the main point: don't interrupt that process block 
    // have to do all the work your plugin has to in a fixed amount of time 
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

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
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // Making audio processor value state tree, which syncs the parameters between the knobs on our GUI and the ones in the DSP
    // It needs to be public so that the editor can access it

    // apvts expects all parameters, so we need a function that will do that - that's what the last parameter for TreeState wants
    static juce::AudioProcessorValueTreeState::ParameterLayout
        createParameterLayout();

    juce::AudioProcessorValueTreeState apvts{ *this, nullptr, "Parameters", createParameterLayout() };

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleEQAudioProcessor)
};
