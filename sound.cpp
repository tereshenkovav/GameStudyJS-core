#include "sound.h"

Sound::Sound(QString filename, Game * game, QObject *parent) : QObject(parent)
{
    this->game = game ;
    buf.loadFromFile(filename.toStdString()) ;
    snd.setBuffer(buf);
}

void Sound::pauseOrResume()
{
    if (snd.getStatus()==snd.Paused)
        snd.play() ;
    else
        if (snd.getStatus()==snd.Playing) snd.pause() ;
}

void Sound::setOn(bool ison)
{
    snd.setVolume(ison?100.0f:0.0f) ;
}

void Sound::play()
{
    snd.setPlayingOffset(sf::Time::Zero) ;
    snd.play();
}

void Sound::stop()
{
    snd.stop() ;
}

void Sound::setLoop(bool isloop)
{
    snd.setLoop(isloop) ;
}

bool Sound::isPlayed() const
{
    return (snd.getStatus()==snd.Playing) ;
}

