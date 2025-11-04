#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
{
    juce::TextButton* buttons[] = {
        &loadButton, &restartButton, &stopButton, &previousButton,
        &playButton, &pauseButton, &nextButton, &loopButton,
        &muteButton, &skipBackButton, &skipForwardButton, &setAButton, &setBButton, &loopABButton ,};


    for (auto* btn : buttons)
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
    }

    addAndMakeVisible(ABloopLabel);
    ABloopLabel.setText("A: 0.0  B: 0.0  Loop: OFF", juce::dontSendNotification);
    ABloopLabel.setJustificationType(juce::Justification::centred);


    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);
    addAndMakeVisible(volumeLabel);
    volumeLabel.setText("Volume", juce::dontSendNotification);
    volumeLabel.setJustificationType(juce::Justification::centredLeft);



    speedSlider.setRange(0.5, 2.0, 0.1);
    speedSlider.setValue(1.0);
    speedSlider.addListener(this);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(speedLabel);
    speedLabel.setText("Speed", juce::dontSendNotification);
    speedLabel.setJustificationType(juce::Justification::centredLeft);

    positionSlider.setRange(0.0, 1.0, juce::dontSendNotification);
    positionSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    positionSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    positionSlider.addListener(this);
    addAndMakeVisible(positionSlider);

    addAndMakeVisible(positionLabel);
    positionLabel.setText("0:00 / 0:00", juce::dontSendNotification);
    positionLabel.setJustificationType(juce::Justification::centred);
    startTimer(100);

    
    addAndMakeVisible(remainingTimeLabel);
    remainingTimeLabel.setText("Remaining: 0:00", juce::dontSendNotification);
    remainingTimeLabel.setJustificationType(juce::Justification::centredRight);
    remainingTimeLabel.setColour(juce::Label::textColourId, juce::Colours::white);


    loadSession();

    addAndMakeVisible(positionLabel);
    positionLabel.setText("0:00 / 0:00", juce::dontSendNotification);
    positionLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(metadataLabel);
    metadataLabel.setJustificationType(juce::Justification::centred);
    metadataLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    metadataLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    metadataLabel.setText("No file loaded", juce::dontSendNotification);

    volumeSlider.setColour(juce::Slider::trackColourId, juce::Colours::beige); 
    volumeSlider.setColour(juce::Slider::thumbColourId, juce::Colours::brown); 

    speedSlider.setColour(juce::Slider::trackColourId, juce::Colours::beige);
    speedSlider.setColour(juce::Slider::thumbColourId, juce::Colours::brown);

    positionSlider.setColour(juce::Slider::trackColourId, juce::Colours::beige);
    positionSlider.setColour(juce::Slider::thumbColourId, juce::Colours::brown);

    


}

PlayerGUI::~PlayerGUI()
{
    saveSession();
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
    g.fillAll(juce::Colours::darksalmon);
    drawWaveform(g);
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


    volumeLabel.setBounds(40, y + 70, 80, 30);
    volumeSlider.setBounds(130, y + 70, getWidth() - 170, 30);

    speedLabel.setBounds(40, y + 120, 80, 30);
    speedSlider.setBounds(130, y + 120, getWidth() - 170, 30);

    positionSlider.setBounds(40, y + 170, getWidth() - 80, 20);
    positionLabel.setBounds(40, y + 200, getWidth() - 80, 30);

    
    int labelY = positionLabel.getY();
    int labelHeight = positionLabel.getHeight();
    int labelWidth = 80;

    
    positionLabel.setBounds(40, labelY, labelWidth, labelHeight);
    remainingTimeLabel.setBounds(getWidth() - labelWidth - 40, labelY, labelWidth, labelHeight);



    int abButtonWidth = 80;
    int abButtonHeight = 40;
    int abSpacing = 10;


    int abTotalWidth = (abButtonWidth * 3) + (abSpacing * 2);
    int abStartX = (getWidth() - abTotalWidth) / 2;
    int abY = y + buttonHeight + 220;

    setAButton.setBounds(abStartX, abY, abButtonWidth, abButtonHeight);
    setBButton.setBounds(abStartX + abButtonWidth + abSpacing, abY, abButtonWidth, abButtonHeight);
    loopABButton.setBounds(abStartX + (abButtonWidth + abSpacing) * 2, abY, abButtonWidth, abButtonHeight);

    int centerX = getWidth() / 2;
    skipBackButton.setBounds(centerX - 100 - 50, positionLabel.getY(), 80, 50);
    skipForwardButton.setBounds(centerX + 70, positionLabel.getY(), 80, 50);

    metadataLabel.setBounds(30, y + 200, 200, 120);

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
    if (button == &skipBackButton){
        playerAudio.seekBy(-10.0);
    }

    if (button == &skipForwardButton){
        playerAudio.seekBy(10.0);
    }
    if (button == &setAButton)
    {
        point_A = playerAudio.getPosition();
        setAButton.setButtonText("A: " + juce::String(point_A, 1));
    }

    if (button == &setBButton)
    {
        point_B = playerAudio.getPosition();
        setBButton.setButtonText("B: " + juce::String(point_B, 1));
    }

    if (button == &loopABButton)
    {
        loop_AB = !loop_AB;
        loopABButton.setButtonText(loop_AB ? "Looping A-B" : "Loop A-B");
    }
    if (button == &loadButton){
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

                        juce::AudioFormatManager fm;
                        fm.registerBasicFormats();
                        if (auto* reader = fm.createReaderFor(file))
                        {
                            juce::String text;
                            auto& meta = reader->metadataValues;

                            if (meta.size() > 0)
                            {
                                for (auto& key : meta.getAllKeys())
                                    text << key << ": " << meta[key] << "\n";
                            }
                            else
                            {
                                text = "Title: " + file.getFileName();
                            }

                            double duration = reader->lengthInSamples / reader->sampleRate;
                            text << "\nDuration: " << juce::String(duration, 2) << " seconds";

                            metadataLabel.setText(text, juce::dontSendNotification);
                            delete reader;
                        }
                        else
                        {
                            metadataLabel.setText("Could not read metadata.", juce::dontSendNotification);
                        }
                    }
                }
            });
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
    repaint();

    
    

   
    double remaining = len - pos;
    int remMin = (int)(remaining / 60);
    int remSec = (int)remaining % 60;

    juce::String remainingText = juce::String::formatted("-%d:%02d", remMin, remSec);
    remainingTimeLabel.setText(remainingText, juce::dontSendNotification);




    if (loop_AB && point_B > point_A && playerAudio.getPosition() >= point_B)
    {
        playerAudio.setPosition(point_A);
    }

}
void PlayerGUI::drawWaveform(juce::Graphics& g)
{
    auto& thumbnail = playerAudio.getThumbnail();

    if (thumbnail.getTotalLength() > 0.0)
    {
        g.setColour(juce::Colours::beige);
        auto area = getLocalBounds().reduced(40, 300);
        area = area.translated(0, 200);
        thumbnail.drawChannel(g, area, 0.0, thumbnail.getTotalLength(), 0, 1.0f);

        double pos = playerAudio.getCurrentPosition();
        double ratio = pos / thumbnail.getTotalLength();
        int x = static_cast<int>(area.getX() + ratio *area.getWidth());

        g.setColour(juce::Colours::darkblue);
        g.drawLine((float)x, (float)area.getY(), (float)x, (float)area.getBottom(), 2.0f);
    }
    else
    {
        g.setColour(juce::Colours::white);
        g.drawFittedText("No waveform loaded", getLocalBounds(), juce::Justification::centred, 1);
    }
}
void PlayerGUI::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    repaint();
}
void PlayerGUI::saveSession()
{
    juce::File sessionFile = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory)
        .getChildFile("last_session.txt");

    if (playerAudio.getFile().existsAsFile())
    {
        lastPosition = playerAudio.getPosition();

        juce::String content = playerAudio.getFile().getFullPathName()
            + "\n" + juce::String(playerAudio.getPosition());

        if (sessionFile.replaceWithText(content))
            DBG("Saved session to: " + sessionFile.getFullPathName());
        else
            DBG("Failed to save session to: " + sessionFile.getFullPathName());
    }
    else
    {
        DBG("No current file to save for session.");
    }


}
void PlayerGUI::loadSession()
{
    juce::File sessionFile = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory)
        .getChildFile("last_session.txt");

    if (!sessionFile.existsAsFile())
    {
        DBG("No session file found at: " + sessionFile.getFullPathName());
        return;
    }

    juce::StringArray lines;
    sessionFile.readLines(lines);

    if (lines.size() < 2)
    {
        DBG("Invalid session file (needs 2 lines)");
        return;
    }

    juce::File lastFile(lines[0].trim());
    double lastPosition = lines[1].trim().getDoubleValue();

    if (!lastFile.existsAsFile())
    {
        DBG("Saved file not found: " + lastFile.getFullPathName());
        return;
    }

    DBG("Trying to load last file: " + lastFile.getFullPathName());

    if (playerAudio.loadFile(lastFile))
    {
        DBG("File loaded, waiting to restore position...");


        juce::Timer::callAfterDelay(1000, [this, lastPosition]()
            {
                double len = playerAudio.getLength();
                DBG("File length detected: " + juce::String(len));
                if (len > 0.0)
                {
                    playerAudio.setPosition(lastPosition);
                    positionSlider.setRange(0.0, len, juce::dontSendNotification);
                    positionSlider.setValue(lastPosition, juce::dontSendNotification);
                    DBG("Restored last position: " + juce::String(lastPosition));
                }
                else
                {
                    DBG("File length still 0. Could not restore position.");
                }
            });
    }
    else
    {
        DBG("playerAudio.loadFile failed");
    }
}

