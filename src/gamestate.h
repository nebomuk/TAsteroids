#ifndef GAMESTATE_H
#define GAMESTATE_H
#include <QObject>

// this classed is used by the scripts to save its state if the script crashes
class GameState : public QObject
{
	Q_OBJECT

	Q_PROPERTY(int phase WRITE setPhase READ phase)
    Q_PROPERTY(bool initialPhase READ initialPhase WRITE setInitialPhase NOTIFY signalPhaseChanged)
	Q_PROPERTY(int playerCount READ playerCount WRITE setPlayerCount)
    Q_PROPERTY(bool gameOver READ gameOver WRITE setGameOver) // game does not continue, either completed or lost

public:
    GameState(QObject * parent = Q_NULLPTR);
    void setPhase(int phase);
    int phase() const;
    void setInitialPhase(int b);
    int initialPhase() const;
    void setPlayerCount(int playerCount);
    int playerCount() const;

    bool gameOver() const;
    void setGameOver(bool gameOver);

signals:
    void signalPhaseChanged(int);
private:
    int phase_;
	int initialPhase_;
	int playerCount_;
    bool gameOver_; // set to true when game over
};

#endif





