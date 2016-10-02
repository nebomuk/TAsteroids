//#include "sdlsound.h"
////#define USE_QDEBUG
//#ifdef USE_QDEBUG
//#include <QtGlobal>
//#endif

//// initialization of static Members
//int SDLSound::instanceCounter = 0;
//bool SDLSound::initialized = false;

///*static*/ void SDLSound::init()
//{
//	if (instanceCounter <= 0 && !initialized)
//	{
//		// 2 means stereo audio
//		initialized = true;
//		Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096 );
//		Mix_AllocateChannels(32);
//		//Mix_ChannelFinished(onChannelFinished); // enable callback
//		//printf("SDL_Mixer initialized\n");
//	}
//}

///*static*/ void SDLSound::deInit()
//{
//	 // check if there's no instance of SDLSound left
//	// before audio device is closed
//	if (instanceCounter <= 0 && initialized)
//	{
//		initialized = false;
//		Mix_ChannelFinished(NULL); // disable callback
//		Mix_CloseAudio();
//		//printf("SDL_Mixer deinitialized\n");
//	}
//}


//SDLSound::SDLSound() :
//chunk(NULL),
//fileName(NULL),
//numLoops(0), // play once
//lastMixChannel(-1)
//{
//	init();
//	++instanceCounter;
//}

//SDLSound::~SDLSound()
//{
//	Mix_FreeChunk(chunk);
//		delete[] fileName;
//	--instanceCounter;
//	deInit();
//	//printf("SDLSound destroyed\n");
//}

//void SDLSound::play()
//{
//	if (!chunk)
//		fprintf(stderr, "Unable to play sound file: No file loaded");
//	else
//	{
//		// let SDL_Mixer choose a channel: -1
//		// -1 is returned on error
//		lastMixChannel = Mix_PlayChannel( -1, chunk, numLoops );
//		if (lastMixChannel == -1)
//		{
//					#ifdef USE_QDEBUG
//					qDebug("Unable to play sound file: %s", Mix_GetError());
//					#else
//					fprintf(stderr, "Unable to play sound file: %s\n", Mix_GetError());
//					#endif
//		}
//	}
//}

//void SDLSound::stop()
//{
//	if(lastMixChannel != -1)
//	Mix_HaltChannel(lastMixChannel);
//}

///*static*/ void	SDLSound::stopAll()
//{
//   Mix_HaltChannel(-1);
//}

//void SDLSound::setFile (const char* cstr)
//{
//	delete[] fileName;
//   fileName = new char[strlen(cstr)+1];
//	strcpy(fileName,cstr);
//		chunk = Mix_LoadWAV(fileName);
//	if (chunk == NULL)
//		{
//			#ifdef USE_QDEBUG
//			qDebug("Unable to load WAV file %s: %s", fileName, Mix_GetError());
//			#else
//			fprintf(stderr, "Unable to load WAV file %s: %s\n", fileName, Mix_GetError());
//			#endif
//		 }
//}
