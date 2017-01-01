#ifndef MENUGAMEINTERACTION_H
#define MENUGAMEINTERACTION_H

#include "graphicsview.h"

#include <QObject>
#include <QQmlApplicationEngine>


///
/// \brief The MenuGameInteraction starts the game (widget based) from the main menu (qml based) and displays
/// the main menu when the game is shut down
///
class MenuGameInteraction : public QObject
{
    Q_PROPERTY(int playerCount MEMBER playerCount_)
    Q_PROPERTY(int highScore MEMBER highScore_ NOTIFY _) // constant to avoid qml warnings



    Q_OBJECT
public:
    explicit MenuGameInteraction(QObject *parent = 0);

    virtual ~MenuGameInteraction();

    Q_INVOKABLE
    void showMainMenu();

    Q_INVOKABLE
    void showGame();

signals:
    void _();

public slots:

private slots:
    void slotHighScore(int score); // receives the highScore from the gameGraphicsView when the game window is closed

private:
    QQmlApplicationEngine * engine_;
    GraphicsView * gameGraphicsView_;
    int playerCount_;
    int highScore_; // when != 0, used to show the "enter player name" screen in qml
};

#endif // MENUGAMEINTERACTION_H
