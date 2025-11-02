#pragma once
#include <JuceHeader.h>

class PlayerAudio
{
public:
    PlayerAudio();
    ~PlayerAudio();

    // Audio setup
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

    // Playback control
    bool loadFile(const juce::File& file);
    void start();
    void stop();
    void setGain(float gain);
    void setPosition(double pos);
    double getPosition() const;
    double getLength() const;

    // Extra features
    void toggleMute();
    void setLooping(bool shouldLoop);
    bool getLooping() const { return isLooping; }
    void setSpeed(double speed);

    juce::AudioThumbnail& getThumbnail() { return thumbnail; }
    double getCurrentPosition() const { return transportSource.getCurrentPosition(); }
    juce::AudioFormatManager& getFormatManager() { return formatManager; }

    void seekBy(double seconds);
    bool isPlaying() const;




private:
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;

    juce::AudioThumbnailCache thumbnailCache{10};
    juce::AudioThumbnail thumbnail{512, formatManager, thumbnailCache};

    float prev = 1.0f;   
    bool muted = false;        
    bool isLooping = false;    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)
};
