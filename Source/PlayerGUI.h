#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"

class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::Timer,
    public juce::ChangeListener
{
public:
    PlayerGUI();
    ~PlayerGUI() override;
    void sliderValueChanged(juce::Slider* slider) override;
    void buttonClicked(juce::Button* button) override;
    void timerCallback() override;

    void resized() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();
    void paint(juce::Graphics& g) override;
    void drawWaveform(juce::Graphics& g);
    void changeListenerCallback(juce::ChangeBroadcaster*source) override;


private:
    PlayerAudio playerAudio;


    juce::TextButton loadButton{ "Load File" };
    juce::TextButton restartButton{ "Restart" };
    juce::TextButton stopButton{ "Stop" };
    juce::TextButton previousButton{ "Previous" };
    juce::TextButton playButton{ "Play" };
    juce::TextButton pauseButton{ "Pause" };
    juce::TextButton nextButton{ "Next" };
    juce::TextButton loopButton{ "Loop" };
    juce::TextButton muteButton{ "Mute" };
    juce::TextButton skipBackButton{ "<- 10s" };
    juce::TextButton skipForwardButton{ "->"
                                        " 10s" };



    juce::Slider volumeSlider;
    juce::Slider speedSlider;
    juce::Slider positionSlider;
    juce::Label positionLabel;

    juce::AudioThumbnailCache thumbnailCache{ 10 };
    juce::AudioThumbnail thumbnail{ 512, playerAudio.getFormatManager(), thumbnailCache };

    bool fileLoaded = false;
    double lastPosition = 0.0;




    std::unique_ptr<juce::FileChooser> fileChooser;




    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};
// New feature added by Ahmed
// New feature added by Ahmed
