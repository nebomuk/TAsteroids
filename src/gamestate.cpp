#include "gamestate.h"

GameState::GameState(QObject *parent) : QObject(parent), phase_(0), initialPhase_(0), playerCount_(1), gameOver_(false) {}

void GameState::setPhase(int phase){phase_ = phase; emit signalPhaseChanged(phase);}

int GameState::phase() const { return phase_;}

void GameState::setInitialPhase(int b){initialPhase_ = b;}

int GameState::initialPhase() const { return initialPhase_;}

void GameState::setPlayerCount(int playerCount){ playerCount_ = playerCount; }

int GameState::playerCount() const { return playerCount_; }

bool GameState::gameOver() const
{
    return gameOver_;
}

void GameState::setGameOver(bool gameOver)
{
    gameOver_ = gameOver;
}
