#include "sound.h"

Sound::Sound()
{
    for(int i = 0; i< 3; ++i)
    {
        soundeffects_.append(new QSoundEffect());
    }
    cyclicCounter = 0;
}

Sound::~Sound()
{
    stop();
    qDeleteAll(soundeffects_);
    soundeffects_.clear();
}


void Sound::play()
{
    QSoundEffect * soundeffect = soundeffects_.at(cyclicCounter);
    if(cyclicCounter < soundeffects_.size()-1)
    {
        ++cyclicCounter;
    }
    else
    {
        cyclicCounter = 0;
    }
    soundeffect->play();
}

void Sound::stop()
{
    for(QSoundEffect * soundeffect : soundeffects_)
    {
         soundeffect->stop();
    }
}

void Sound::stopAll()
{
    for(QSoundEffect *soundeffect : soundeffects_)
    {
         soundeffect->stop();
    }
}

const char *Sound::file() const
{
    return soundeffects_.first()->source().toLocalFile().toLocal8Bit().constData();
}


void Sound::setFile(const char *cstr)
{
    for(QSoundEffect * soundeffect : soundeffects_)
    {
        soundeffect->setSource(QUrl::fromLocalFile(QString::fromLocal8Bit(cstr)));
    }
}

int Sound::loops() const
{
    if(!soundeffects_.isEmpty())
    {
        return soundeffects_.first()->loopCount();
    }
    return 0;
}

void Sound::setLoops(int number)
{
    if(number == -1)
    {
        for(QSoundEffect *soundeffect : soundeffects_)
        {
            soundeffect->setLoopCount(QSoundEffect::Infinite);
        }
    }
    else
    {
        for(QSoundEffect * soundeffect : soundeffects_)
        {
            soundeffect->setLoopCount(number);
        }
    }
}
