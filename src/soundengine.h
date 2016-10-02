#ifndef SOUNDENGINE_H
#define SOUNDENGINE_H

#include <QHash>
#include <QFile>
#include <QDir>
#include <QTemporaryFile>
#include <QStringList>

class ISound;

/*
 SoundEngine can load .wav files from the disk or from a qt-resource
 these files are stored in temporary files in the system's temp directory
 it uses SDL for playing sounds but any backend can be used as long as it inherits
 the ISound interface and allows multiple calls to play()
 */

// soundFiles passed to loadSounds MUST NOT have any QFile handle from anywhere
// because e.g. Windows blocks read/write access to files that are already opened
// it's not enough to call QFile::close because access will still be blocked

class SoundEngine : public QObject
{
	Q_OBJECT
	Q_PROPERTY(int loopDuplicateCount READ loopDuplicateCount WRITE setLoopDuplicateCount)
public:
	SoundEngine(QObject * parent = NULL);
	~SoundEngine();

public slots:
	// pass -1 to loop infinitely
	// note that a loop count of 0 means the sound plays exactly once (unlike QSound, which plays once for 1)
	void play(const QString& filename, int loops = 0);
	void stop(const QString& filename);
	void stopAll();

	// sounds that start with loop_ are added loopDuplicateCount_ times to the soundEngine with a suffix
	// like that: loop_sound.wav_0, loop_sound.wav_1. They then can be played and stopped seperately
	void setLoopDuplicateCount(int loopDuplicateCount){ loopDuplicateCount_ = loopDuplicateCount; }
	int loopDuplicateCount() const { return loopDuplicateCount_; }

	// copys the files in soundFiles into the system's temp directory
	// and makes them available to the SoundEngine with their names (without path)
	// this is necessary for resource files, because the SDL Sound backend doesn't support Qt-resources
	void loadSounds(const QStringList& soundFiles);

private:
	QHash<QString,ISound*> soundPlayers_;
	QStringList tempFileNames_;
	int loopDuplicateCount_;
};

#endif // SOUNDENGINE_H
