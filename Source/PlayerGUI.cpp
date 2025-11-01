#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
{
    // ÅÖÇÝÉ ÌãíÚ ÇáÃÒÑÇÑ ãÚðÇ
    for (auto* btn : { &loadButton, &restartButton, &stopButton, &previousButton,
                       &playButton, &pauseButton, &nextButton, &loopButton, &muteButton })
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
    }

    // ÅÚÏÇÏ ÇáÓáÇíÏÑ (ÇáÊÍßã Ýí ÇáÕæÊ)
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);

    speedSlider.setRange(0.5, 2.0, 0.1);
    speedSlider.setValue(1.0);
    speedSlider.addListener(this);
    addAndMakeVisible(speedSlider);


    progressSlider.setRange(0.0, 1.0);
    progressSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    progressSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    progressSlider.addListener(this);
    addAndMakeVisible(progressSlider);

    currentTimeLabel.setText("0:00", juce::dontSendNotification);
    totalTimeLabel.setText("0:00", juce::dontSendNotification);
    addAndMakeVisible(currentTimeLabel);
    addAndMakeVisible(totalTimeLabel);
    startTimerHz(20);


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

    int totalButtons = 9; // ÚÏÏ ÇáÃÒÑÇÑ
    int totalWidth = (buttonWidth * totalButtons) + (spacing * (totalButtons - 1));

    int startX = (getWidth() - totalWidth) / 2;
    int y = 30;
    int x = startX;

    // ÊÑÊíÈ ÇáÃÒÑÇÑ Ýí ÕÝ æÇÍÏ
    loadButton.setBounds(x, y, buttonWidth, buttonHeight); x += buttonWidth + spacing;
    restartButton.setBounds(x, y, buttonWidth, buttonHeight); x += buttonWidth + spacing;
    stopButton.setBounds(x, y, buttonWidth, buttonHeight); x += buttonWidth + spacing;
    previousButton.setBounds(x, y, buttonWidth, buttonHeight); x += buttonWidth + spacing;
    playButton.setBounds(x, y, buttonWidth, buttonHeight); x += buttonWidth + spacing;
    pauseButton.setBounds(x, y, buttonWidth, buttonHeight); x += buttonWidth + spacing;
    nextButton.setBounds(x, y, buttonWidth, buttonHeight); x += buttonWidth + spacing;
    loopButton.setBounds(x, y, buttonWidth, buttonHeight); x += buttonWidth + spacing;
    muteButton.setBounds(x, y, buttonWidth, buttonHeight);

    // ÇáÓáÇíÏÑ ÊÍÊ ÇáÃÒÑÇÑ
    volumeSlider.setBounds(40, y + 70, getWidth() - 80, 30);
    speedSlider.setBounds(40, y + 120, getWidth() - 80, 30);
    progressSlider.setBounds(80, y + 200, getWidth() - 160, 20);
    currentTimeLabel.setBounds(20, y + 200, 50, 20);
    totalTimeLabel.setBounds(getWidth() - 70, y +200, 50, 20);


}

void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        fileChooser = std::make_unique<juce::FileChooser>(
            "Select audio file...",
            juce::File{},
            "*.wav;*.mp3");

        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file.existsAsFile())
                    playerAudio.loadFile(file);
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
    if (slider == &speedSlider) {
        playerAudio.setSpeed(slider->getValue());
    }if (slider == &progressSlider && playerAudio.getLength() > 0){
        double pos = progressSlider.getValue() * playerAudio.getLength();
        playerAudio.setPosition(pos);
    }
}

void PlayerGUI::timerCallback(){
    double position = playerAudio.getPosition();
    double length = playerAudio.getLength();

    if (length > 0.0)
    {
        progressSlider.setValue(position / length, juce::dontSendNotification);

        auto formatTime = [](double seconds) {
            int totalSecs = (int)seconds;
            int mins = totalSecs / 60;
            int secs = totalSecs % 60;
            return juce::String::formatted("%d:%02d", mins, secs);
        };

        currentTimeLabel.setText(formatTime(position), juce::dontSendNotification);
        totalTimeLabel.setText(formatTime(length), juce::dontSendNotification);
    }
}


