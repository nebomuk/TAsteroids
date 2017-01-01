#ifndef MAINWINDOWIMPL_H
#define MAINWINDOWIMPL_H


#include <QKeyEvent>
#include <QGraphicsView>
#include <QBasicTimer>
#include <QPointer>
#include <QTime>
#include <QGraphicsProxyWidget>
#include "Box2D/Box2D.h"
#include "gamestate.h"
#include "globalvariables.h"
#include "centerrectinrect.h"
#include "QGraphicsBox2DPolygonItem.h"

/*
 this is the game's main widget which instanciates all other classes.

 if you create an instance of GraphicsView, you must call restart() to spawn items

 */
class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(QWidget * parent = 0);
    ~GraphicsView();
    int lastHighScore() const; // returns the value of the highScoreCounter item
    // lastHighscore_ is updated when clear() is called (the window closes)

    int playerCount() const;
    void setPlayerCount(int playerCount);

public slots:

    // (re) starts the game
    void restart(); // calls clear and populate after events have been processed
    void onApplicationStateChanged(Qt::ApplicationState state);

private slots:

    // pauses the game when true, resumes when false
    void setPaused(bool b); // Pause key
    bool isPaused();
    void toggleFullScreen(); // F11 key

    // after clear, events have to be processed before populate e.g. QTimer(0,...)
    void clear(); // clears the scene
    void populate(); // fills the scene with items

signals:
    // this signal is emitted when closeEvent() is called
    void signalClose();

protected:

    // experimental function to add physics rects
    QGraphicsRectItem * addRect(QPointF pos, QSizeF size);

    // events
    virtual void timerEvent(QTimerEvent* event) Q_DECL_OVERRIDE;
    virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    virtual void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    virtual void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    virtual bool viewportEvent(QEvent *event) Q_DECL_OVERRIDE;

    // save window geometry
    void closeEvent(QCloseEvent *event);

    // keep player inside visible area
    void wrapSprite(MovingItem * item);

    // initialization functions
    void setupScript();

    // read and save window position & size
    void readSettings();
    void writeSettings();

signals:
    // these signals are emitted by keyPressEvent /keyReleaseEvent
    void signalKeyPress(int);
    void signalKeyRelease(int);

private:
    GraphicsEngine * graphicsEngine;
    QBasicTimer *timer;
    QString dataLocation;
    QList<Vehicle*> playerVehicles_;
    QList<AnimatedItem*> hitpointBars_;
    MechanicalCounter * highScoreCounter_;

    QList<GraphicsSoftButton* > leftSoftButtons_;
    QList<GraphicsSoftButton* > rightSoftButtons_;

    JSProxy* scriptProxy;


    // distance between the window borders (than can be resized, without keeping aspect ratio)
    //and the scene rect, which has a constant size
    QPointF borderSceneRectDist_;

    QList<QGraphicsBox2DPolygonItem*> bodyItems_;
    QList<b2Body*> groundBodies_;
    int lastHighScore_;
    bool gameOver_; // set to true when game over

    bool doubleBackToExitPressedOnce_;

    // create some colored fun polygons that you can push around
    void addPolygons();
    void adjustSoftButtonPositions();

};
#endif




