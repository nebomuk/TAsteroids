//#ifndef SDLSOUND_H
//#define SDLSOUND_H

//#include "isound.h"
//#if !defined(__WIN32__)
//#include "SDL/SDL_mixer.h"
//#else
//#include "SDL_mixer.h"
//#endif
//// this class uses an instanceCounter to initialize and deinitialize SDL_Mixer.
//// the static play method initializes SDL_Mixer when its called
//// and uses the Mix_ChannelFinished callback to deinitialize SDL_Mixer

//class SDLSound : public ISound
//{
//public:
//	SDLSound();
//	virtual ~SDLSound();

//	void                play(); // play a sound that has been specified by file
//	void                stop(); // this stops only the sound that has been playing since the last call to play
//	static void			stopAll(); // stops all sounds
//	const char*         file() const {  return fileName;}
//	void                setFile (const char* cstr);
//	int                 loops() const { return numLoops;}
//	void                setLoops ( int number ) { numLoops = number;}

//protected:

//	// initializes SDL_Mixer
//	static void init();
//	// deinitalizes SDL_Mixer
//	static void deInit();

//private:
//	Mix_Chunk *chunk;			// for play method
//		char * fileName;
//	int numLoops;

//	// internal channel for SDL audio mixer
//	// mixChannel is current playing channel of this object
//	int lastMixChannel;


//	// this counter is increased every time a new instance of SDLSound is created
//	// and decreased if an instance is destroyed. It is also increased and decreased by the static play method
//	static int instanceCounter;

//	static bool initialized; // check wheter SDL_Mixer has already been initialized

//	// disable copy
//	SDLSound& operator=(const SDLSound& other);
//	SDLSound(const SDLSound& other);
//};

//#endif // SDLSOUND_H
