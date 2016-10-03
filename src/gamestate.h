#ifndef GAMESTATE_H
#define GAMESTATE_H
#include <QObject>

// this classed is used by the scripts to save its state if the script crashes
class GameState : public QObject
{
	Q_OBJECT

	Q_PROPERTY(int phase WRITE setPhase READ phase)
	Q_PROPERTY(bool initialPhase READ initialPhase WRITE setInitialPhase)
	Q_PROPERTY(int playerCount READ playerCount WRITE setPlayerCount)

public:
    GameState(QObject * parent = Q_NULLPTR) : QObject(parent), phase_(0), initialPhase_(0), playerCount_(1) {}
	void setPhase(int phase){phase_ = phase; emit signalPhaseChanged(phase);}
	int phase() const { return phase_;}
	void setInitialPhase(int b){initialPhase_ = b;}
	int initialPhase() const { return initialPhase_;}
	void setPlayerCount(int playerCount){ playerCount_ = playerCount; }
	int playerCount() const { return playerCount_; }

signals:
	void signalPhaseChanged(int);
private:
	int phase_;
	int initialPhase_;
	int playerCount_;
};

#endif
