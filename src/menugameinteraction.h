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

    Q_OBJECT
public:
    explicit MenuGameInteraction(QObject *parent = 0);

    virtual ~MenuGameInteraction();

    Q_INVOKABLE
    void showMainMenu();

    Q_INVOKABLE
    void showGame();

public slots:

private:
    QQmlApplicationEngine * engine_;
    GraphicsView * gameGraphicsView_;
    int playerCount_;
};

#endif // MENUGAMEINTERACTION_H
