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
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    void saveSession();
    void loadSession();



private:
    PlayerAudio playerAudio;

    
    juce::String shortenText(const juce::String& text, int maxLength);

   
    juce::ListBox playlistBox;
    std::vector<juce::File> playlistFiles;
    int currentPlaylistIndex = -1;

    juce::TextButton addToPlaylistButton{ "Add to Playlist" };
    juce::TextButton removeFromPlaylistButton{ "Remove" };
    juce::TextButton nextTrackButton{ "Next Track" };
    juce::TextButton previousTrackButton{ "Prev Track" };

    
    class PlaylistModel : public juce::ListBoxModel
    {
    public:
        PlaylistModel(PlayerGUI& owner) : owner(owner) {}

        int getNumRows() override
        {
            return owner.playlistFiles.size();
        }

        void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override
        {
            if (rowIsSelected)
                g.fillAll(juce::Colours::lightblue);
            else
                g.fillAll(juce::Colours::white);

            g.setColour(juce::Colours::black);
            g.setFont(14.0f);

            if (rowNumber < owner.playlistFiles.size())
            {
                juce::String displayText = juce::String(rowNumber + 1) + ". " + owner.playlistFiles[rowNumber].getFileName();
                if (rowNumber == owner.currentPlaylistIndex)
                    displayText = "? " + displayText;

                g.drawText(displayText, 5, 0, width - 5, height, juce::Justification::centredLeft);
            }
        }

        void listBoxItemClicked(int row, const juce::MouseEvent&) override
        {
            owner.loadPlaylistFile(row);
        }

    private:
        PlayerGUI& owner;
    };

    std::unique_ptr<PlaylistModel> playlistModel;

    void updatePlaylist();
    void loadPlaylistFile(int index);
    

    juce::TextButton loadButton{ "Load File" };
    juce::TextButton restartButton{ "Restart" };
    juce::TextButton stopButton{ "Stop" };
    juce::TextButton previousButton{ "<< previous" };
    juce::TextButton playButton{ "Play" };
    juce::TextButton pauseButton{ "Pause || " };
    juce::TextButton nextButton{ "Next >>" };
    juce::TextButton loopButton{ "Loop" };
    juce::TextButton muteButton{ "Mute" };
    juce::TextButton skipBackButton{ "<- 10s" };
    juce::TextButton skipForwardButton{ "-> 10s" };
    juce::TextButton setAButton{ "Set A" };
    juce::TextButton setBButton{ "Set B" };
    juce::TextButton loopABButton{ "Loop A-B" };




    juce::Slider volumeSlider;
    juce::Slider speedSlider;
    juce::Label volumeLabel;
    juce::Label speedLabel;
    juce::Slider positionSlider;
    juce::Label remainingTimeLabel;
    juce::Label positionLabel;
    juce::Label ABloopLabel;
    juce::Label metadataLabel;



    juce::AudioThumbnailCache thumbnailCache{ 10 };
    juce::AudioThumbnail thumbnail{ 512, playerAudio.getFormatManager(), thumbnailCache };

    bool fileLoaded = false;
    double lastPosition = 0.0;
    double point_A = 0.0;
    double point_B = 0.0;
    bool loop_AB = false;


    std::unique_ptr<juce::FileChooser> fileChooser;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};