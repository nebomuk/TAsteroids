#ifndef MOCKSOUND_H
#define MOCKSOUND_H

#include "isound.h"
#include <QtGlobal>



class MockSound : public ISound
{
public:
    MockSound();

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

#endif // MOCKSOUND_H
