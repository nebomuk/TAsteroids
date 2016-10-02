#include "mocksound.h"

MockSound::MockSound()
{

}


void MockSound::play()
{
}

void MockSound::stop()
{
}

void MockSound::stopAll()
{

}

const char *MockSound::file() const
{
    return Q_NULLPTR;
}

void MockSound::setFile(const char *cstr)
{
}

int MockSound::loops() const
{
    return 0;
}

void MockSound::setLoops(int number)
{
}
