#ifndef SOUND_H
#define SOUND_H

#include "isound.h"
#include <QSoundEffect>


///
/// \brief The Sound class is a QSoundEffect based implementation of ISound
///
class Sound : public ISound
{

private:
    QList<QSoundEffect*> soundeffects_;
    int cyclicCounter;

public:
    Sound();
    virtual ~Sound();

    // ISound interface
public:
    virtual void play() Q_DECL_OVERRIDE;
    virtual void stop() Q_DECL_OVERRIDE;
    virtual void stopAll() Q_DECL_OVERRIDE;
    virtual const char *file() const Q_DECL_OVERRIDE;
    virtual void setFile(const char *cstr) Q_DECL_OVERRIDE;
    virtual int loops() const Q_DECL_OVERRIDE;
    virtual void setLoops(int number) Q_DECL_OVERRIDE;
};

#endif // SOUND_H
