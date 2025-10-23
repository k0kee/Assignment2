#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
{
    // Add buttons
   for (auto* btn : { &loadButton })
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
    }

   nextImage = juce::ImageFileFormat::loadFrom(BinaryData::next_png,BinaryData::next_pngSize);
   next.setImages(false, true, true,
       nextImage, 1.0f, juce::Colours::transparentBlack,
       nextImage, 1.0f, juce::Colours::white.withAlpha(0.3f),
       nextImage, 1.0f, juce::Colours::white.withAlpha(0.6f)
   );

   next.addListener(this);
   addAndMakeVisible(next);

   previousImage = juce::ImageFileFormat::loadFrom(BinaryData::previous_png, BinaryData::previous_pngSize);
   previous.setImages(false, true, true,
       previousImage, 1.0f, juce::Colours::transparentBlack,
       previousImage, 1.0f, juce::Colours::white.withAlpha(0.3f),
       previousImage, 1.0f, juce::Colours::white.withAlpha(0.6f)
   );

   previous.addListener(this);
   addAndMakeVisible(previous);


   playImage = juce::ImageFileFormat::loadFrom(BinaryData::play_png, BinaryData::play_pngSize);
   pauseImage = juce::ImageFileFormat::loadFrom(BinaryData::pause_png, BinaryData::pause_pngSize);
   playAndPause.setImages(false, true, true,
       playImage, 1.0f, juce::Colours::transparentBlack,
       playImage, 1.0f, juce::Colours::black.withAlpha(0.3f),
       pauseImage, 1.0f, juce::Colours::transparentBlack
       );

   playAndPause.addListener(this);
   addAndMakeVisible(playAndPause);

    // Volume slider
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);
}

PlayerGUI::~PlayerGUI()
{

}
void PlayerGUI::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    playerAudio.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerGUI::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    playerAudio.getNextAudioBlock(bufferToFill);
}

void PlayerGUI::releaseResources()
{
    playerAudio.releaseResources();
}
void PlayerGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
}
void PlayerGUI::resized()
{
    int y = 20;
    loadButton.setBounds(20, y, 60, 20);
    playAndPause.setBounds(140, y, 20, 20);
    next.setBounds(180, y, 20, 20);
    previous.setBounds(100, y, 20, 20);

    volumeSlider.setBounds(20, 100, getWidth() - 40, 30);
}
void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        juce::FileChooser chooser("Select audio files...",
            juce::File{},
            "*.wav;*.mp3");

        fileChooser = std::make_unique<juce::FileChooser>(
            "Select an audio file...",
            juce::File{},
            "*.wav;*.mp3");

        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file.existsAsFile())
                {
                    playerAudio.loadFile(file);
                    playAndPause.setImages(false, true, true,
                        pauseImage, 1.0f, juce::Colours::transparentBlack,
                        pauseImage, 1.0f, juce::Colours::black.withAlpha(0.3f),
                        playImage, 1.0f, juce::Colours::transparentBlack
                    );
                }
            });
    }

    if (button == &playAndPause)
   {
        if (!isPlaying)
        {
            playerAudio.start();
            playAndPause.setImages(false, true, true,
                pauseImage, 1.0f, juce::Colours::transparentBlack,
                pauseImage, 1.0f, juce::Colours::black.withAlpha(0.3f),
                playImage, 1.0f, juce::Colours::transparentBlack
            );
            isPlaying = true;
         
        }
        else
        {
            playerAudio.stop();
            playAndPause.setImages(false, true, true,
                playImage, 1.0f, juce::Colours::transparentBlack,
                playImage, 1.0f, juce::Colours::black.withAlpha(0.3f),
                pauseImage, 1.0f, juce::Colours::transparentBlack
            );
            isPlaying = false;
        }
   }
    if (button == &previous)
    {
        playerAudio.setPosition(0.0);
    }
    if (button == &next)
    {

        playerAudio.setPosition(playerAudio.getPosition());

    }
}
void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
    {
        playerAudio.setGain((float)slider->getValue());
    }
}



