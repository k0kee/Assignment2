#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
{
    for (auto* btn : { &loadButton, &restartButton, &stopButton, &previousButton,
                       &playButton, &pauseButton, &nextButton, &loopButton, &muteButton })
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
    }

    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);

    speedSlider.setRange(0.5, 2.0, 0.1);
    speedSlider.setValue(1.0);
    speedSlider.addListener(this);
    addAndMakeVisible(speedSlider);

    positionSlider.setRange(0.0, 1.0, juce::dontSendNotification);
    positionSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    positionSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    positionSlider.addListener(this);
    addAndMakeVisible(positionSlider);

    addAndMakeVisible(positionLabel);
    positionLabel.setText("0:00 / 0:00", juce::dontSendNotification);
    positionLabel.setJustificationType(juce::Justification::centred);
    startTimer(100);
}

PlayerGUI::~PlayerGUI() {}

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
    int buttonWidth = 80;
    int buttonHeight = 40;
    int spacing = 10;

    int totalButtons = 9;
    int totalWidth = (buttonWidth * totalButtons) + (spacing * (totalButtons - 1));

    int startX = (getWidth() - totalWidth) / 2;
    int y = 30;
    int x = startX;

    loadButton.setBounds(x, y, buttonWidth, buttonHeight); x += buttonWidth + spacing;
    restartButton.setBounds(x, y, buttonWidth, buttonHeight); x += buttonWidth + spacing;
    stopButton.setBounds(x, y, buttonWidth, buttonHeight); x += buttonWidth + spacing;
    previousButton.setBounds(x, y, buttonWidth, buttonHeight); x += buttonWidth + spacing;
    playButton.setBounds(x, y, buttonWidth, buttonHeight); x += buttonWidth + spacing;
    pauseButton.setBounds(x, y, buttonWidth, buttonHeight); x += buttonWidth + spacing;
    nextButton.setBounds(x, y, buttonWidth, buttonHeight); x += buttonWidth + spacing;
    loopButton.setBounds(x, y, buttonWidth, buttonHeight); x += buttonWidth + spacing;
    muteButton.setBounds(x, y, buttonWidth, buttonHeight);

    volumeSlider.setBounds(40, y + 70, getWidth() - 80, 30);
    speedSlider.setBounds(40, y + 120, getWidth() - 80, 30);
    positionSlider.setBounds(40, y + 170, getWidth() - 80, 20);
    positionLabel.setBounds(40, y + 200, getWidth() - 80, 30);
}

void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        fileChooser = std::make_unique<juce::FileChooser>(
            "Select audio file...", juce::File{}, "*.wav;*.mp3");

        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file.existsAsFile())
                {
                    if (playerAudio.loadFile(file))
                    {
                        double len = playerAudio.getLength();
                        if (len > 0.0)
                            positionSlider.setRange(0.0, len, juce::dontSendNotification);
                    }
                }
            });
    }

    if (button == &restartButton)
    {
        playerAudio.stop();
        playerAudio.setPosition(0.0);
        playerAudio.start();
    }

    if (button == &stopButton)
    {
        playerAudio.stop();
        playerAudio.setPosition(0.0);
    }

    if (button == &playButton)
    {
        playerAudio.start();
    }

    if (button == &pauseButton)
    {
        playerAudio.stop();
    }

    if (button == &previousButton)
    {
        playerAudio.setPosition(0.0);
    }

    if (button == &nextButton)
    {
        playerAudio.setPosition(playerAudio.getLength());
    }

    if (button == &loopButton)
    {
        bool newState = !playerAudio.getLooping();
        playerAudio.setLooping(newState);
        loopButton.setButtonText(newState ? "Loop: On" : "Loop: Off");
    }

    if (button == &muteButton)
    {
        playerAudio.toggleMute();
        muteButton.setButtonText(muteButton.getButtonText() == "Mute" ? "Unmute" : "Mute");
    }
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
        playerAudio.setGain((float)slider->getValue());

    else if (slider == &speedSlider)
        playerAudio.setSpeed(slider->getValue());

    else if (slider == &positionSlider)
        playerAudio.setPosition(positionSlider.getValue());
}

void PlayerGUI::timerCallback()
{
    double len = playerAudio.getLength();
    if (len <= 0.0) return;

    double pos = playerAudio.getPosition();

    positionSlider.setRange(0.0, len, juce::dontSendNotification);
    positionSlider.setValue(pos, juce::dontSendNotification);

    int posMin = (int)(pos / 60);
    int posSec = (int)pos % 60;
    int lenMin = (int)(len / 60);
    int lenSec = (int)len % 60;

    juce::String timeText = juce::String::formatted("%d:%02d / %d:%02d",
        posMin, posSec, lenMin, lenSec);
    positionLabel.setText(timeText, juce::dontSendNotification);
}
