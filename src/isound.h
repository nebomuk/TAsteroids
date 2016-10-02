#ifndef ISOUND_H
#define ISOUND_H

//#include <cstdio> // for printf

class ISound
{
public:
	virtual void play()= 0;
	virtual void stop()= 0;
    virtual void stopAll() = 0;
	virtual const char* file() const = 0;
	virtual void setFile (const char* cstr) = 0;
	//virtual bool isFinished () const = 0;
	virtual int loops () const = 0;
	//virtual int loopsRemaining () const = 0;

	// passing -1 will loop infinitely
	virtual void setLoops ( int number ) = 0;
	virtual ~ISound(){/*printf("ISound destroyed\n");*/}
};
// Note: implementation of pure virtual function must be outside class

#endif // ISOUND_H


