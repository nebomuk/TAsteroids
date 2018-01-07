#include "soundengine.h"
#include <QDir>
#include <QMessageBox>
#include "mocksound.h"
#include "sound.h"

SoundEngine::SoundEngine(QObject * parent) : QObject(parent)
{
	loopDuplicateCount_ = 4;
}

void SoundEngine::loadSounds(const QStringList& fileNames)
{
    for(QString sourceFileName : fileNames)
	{
			QString fileName = QFileInfo(sourceFileName).fileName(); // returns file name without path

			// copy the files to the temp directory
			QFile::copy(sourceFileName,QDir::tempPath() + "/tmp_" + fileName);
			QString tempFileName(QDir::tempPath() + "/tmp_" + fileName);

			// save the absolute file names in order to delete them in the dtor
			tempFileNames_ << tempFileName;

			if(fileName.startsWith("loop", Qt::CaseInsensitive))
			{
					for(int i = 0; i< loopDuplicateCount_; ++i)
					{
                                                        ISound * snd = new Sound();
							snd->setFile(qPrintable(QDir::toNativeSeparators(tempFileName))); // required for SDL, on windows C:/path becomes C:\path
							soundPlayers_.insert(fileName+ '_' + QString::number(i),snd); // loop_sound.wav_0, loop_sound.wav_1 etc
					}
			}
                        ISound * snd = new Sound();
			snd->setFile(qPrintable(QDir::toNativeSeparators(tempFileName)));
			soundPlayers_.insert(fileName,snd);
	}

	// fileName is the filename without the path
	// but file.fileName() and temp.fileName() both return the file's name including the full path
}

void SoundEngine::play(const QString& filename, int loops )
{
	ISound * soundPlayer = soundPlayers_.value(filename,NULL);
	if(!soundPlayer)
	{
		qDebug("Sound does not exist: %s",qPrintable(filename));
		return;
	}
		soundPlayer->setLoops(loops);
		soundPlayer->play();
}

void SoundEngine::stop(const QString& filename)
{
	ISound * soundPlayer = soundPlayers_.value(filename,NULL);
	if(!soundPlayer)
	{
		qDebug("Sound does not exist: %s",qPrintable(filename));
		if(filename.startsWith("loop", Qt::CaseInsensitive))
			qDebug("Did you forget the number suffix?");

		return;
	}
		soundPlayer->stop();
}

void SoundEngine::stopAll()
{
    QList<ISound*> sounds = soundPlayers_.values();
    for (ISound * soundPlayer : sounds) {
        soundPlayer->stop();

    }
}

SoundEngine::~SoundEngine()
{
	// if the application crashes here, check for multiple deletes (and multiple inserts of the same new'ed value)
		qDeleteAll(soundPlayers_);

		// destroy the temporary files that have been created before
		bool tempRemoveFailed = false;
        for(QString file : tempFileNames_)
		{
			QDir dir;
			if(!dir.remove(file))
				tempRemoveFailed = true;
		}
		if(tempRemoveFailed)
		{
			QMessageBox::warning(NULL,tr("Could not delete temporary files"),
								 tr("One ore more temporary sound files could not be removed."
									"You can clean up the temp_* files yourself in ") + QDir::tempPath());
		}
}
