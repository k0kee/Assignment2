#include "PlayerAudio.h"

PlayerAudio::PlayerAudio()
{
    formatManager.registerBasicFormats();
}

PlayerAudio::~PlayerAudio()=default;


void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    transportSource.getNextAudioBlock(bufferToFill);

   
    if (isLooping && transportSource.hasStreamFinished())
    {
        transportSource.setPosition(0.0);
        transportSource.start();
    }
}

void PlayerAudio::releaseResources()
{
    transportSource.releaseResources();
}

bool PlayerAudio::loadFile(const juce::File& file)
{
    if (file.existsAsFile())
    {
        if (auto* reader = formatManager.createReaderFor(file))
        {
            transportSource.stop();
            transportSource.setSource(nullptr);
            readerSource.reset();

            readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);

            transportSource.setSource(readerSource.get(),
                0,
                nullptr,
                reader->sampleRate);

            thumbnail.clear();
            thumbnail.setSource(new juce::FileInputSource(file));

            return true;
        }
    }
    return false;
}




void PlayerAudio::start()
{
    transportSource.start();
}

void PlayerAudio::stop()
{
    transportSource.stop();
}

void PlayerAudio::setGain(float gain)
{
    transportSource.setGain(gain);
    prev = gain; 
}

void PlayerAudio::setPosition(double pos)
{
    transportSource.setPosition(pos);
}

double PlayerAudio::getPosition() const
{
    return transportSource.getCurrentPosition();
}

double PlayerAudio::getLength() const
{
    return transportSource.getLengthInSeconds();
}

void PlayerAudio::toggleMute()
{
    if (muted)
    {
        transportSource.setGain(prev); 
        muted = false;
    }
    else
    {
        prev = transportSource.getGain(); 
        transportSource.setGain(0.0f);    
        muted = true;
    }
}

void PlayerAudio::setLooping(bool shouldLoop)
{
    isLooping = shouldLoop;
    if (readerSource)
        readerSource->setLooping(shouldLoop);
}

void PlayerAudio::setSpeed(double speed)
{
    if (readerSource){
        bool wasPlaying = transportSource.isPlaying();
        transportSource.stop();
        transportSource.setSource(readerSource.get(), 0, nullptr,
                                  readerSource->getAudioFormatReader()->sampleRate*speed);
        if(wasPlaying)transportSource.start();
    }
}
void PlayerAudio::seekBy(double seconds){
    double len = getLength();
    if (len <= 0.0)
        return;

    double cur = transportSource.getCurrentPosition();
    double target = cur + seconds;

    if (target < 0.0) target = 0.0;
    if (target > len)  target = len;

    transportSource.setPosition(target);
}

bool PlayerAudio::isPlaying() const{
    return transportSource.isPlaying();
}



