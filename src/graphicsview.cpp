#include <QDesktopServices>
#include <QApplication>
#include <QSettings>
#include <QDir>
#include <QMessageBox>
#include <QEvent>
#include <QGestureEvent>
#include "graphicsview.h"
#include "globalvariables.h"
#include "vehicle.h"
#include "svgcache.h"
#include "graphicsengine.h"
#include "soundengine.h"
#include "graphicssoftbutton.h"
#include "globalvariables.h"
#include "jsproxy.h"
//#include "qgl.h"

#include <QGraphicsColorizeEffect>
#include <QGraphicsSvgItem>
#include <QtDebug>

// note internal resolution is 1600*1200 for positions of items
// all items must be created inside the populate-function
// because scene is cleared after every call to restart

GraphicsView::GraphicsView(QWidget * parent)
	: QGraphicsView(parent)
{
    scriptProxy = Q_NULLPTR;
    graphicsEngine = Q_NULLPTR;
    highScoreCounter_ = Q_NULLPTR;
    doubleBackToExitPressedOnce_ = false;

	this->setWindowTitle("TAsteroids");

	timer = new QBasicTimer;


	// disable scroll bars
	this->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	this->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

	// activates OpenGL
	// if you uncomment this, add opengl to the QT variable in the project file
        //this->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
	// hide cursor
	//setCursor(Qt::BlankCursor);

	// optimizations

	this->setCacheMode(QGraphicsView::CacheBackground);
	/*
	this->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
	this->setOptimizationFlags(QGraphicsView::DontClipPainter
										|QGraphicsView::DontSavePainterState
										|QGraphicsView::DontAdjustForAntialiasing);
	//*/

    // pan and swipe gestures only work with 2 fingers
    QList<Qt::GestureType> gestures;
    gestures  << Qt::PinchGesture;
    for(Qt::GestureType gesture : gestures)
    {
         this->viewport()->grabGesture(gesture);
    }
    viewport()->setAttribute(Qt::WA_AcceptTouchEvents);

	this->setRenderHint(QPainter::Antialiasing,false);

	setStyleSheet("QGraphicsView { border: none }");

    QGraphicsScene * scene = new QGraphicsScene(0.0,-1200.0,1600.0,1200.0,this);
	scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setBackgroundBrush(QPixmap(":images/backgroundRed.png").scaledToWidth(2400));
	setSceneRect(scene->sceneRect());


	graphicsEngine = new GraphicsEngine(this);
	graphicsEngine->setScene(scene);
    readSettings();

	// creates all items
	//restart();

	this->setScene(scene);
}

GraphicsView::~GraphicsView()
{

	clear();
    delete graphicsEngine;
}


/*virtual*/ bool GraphicsView::viewportEvent(QEvent *event)
{
    if (event->type() == QEvent::Gesture)
    {
        QGestureEvent* e = static_cast<QGestureEvent*>(event);
        QList<QGesture*> gestures = e->gestures();
        for(QGesture* gesture : gestures)
        {
            Qt::GestureType type = gesture->gestureType();

            if(type == Qt::PinchGesture)
            {
                QPinchGesture* pinchGesture = static_cast<QPinchGesture*>(gesture);
                bool isPinch = pinchGesture->totalScaleFactor() < 0.7;

                if(gesture->state() == Qt::GestureStarted && isPinch)
                {
                    emit scriptProxy->signalGestureStarted(gesture->gestureType());
                }
                else if(gesture->state() == Qt::GestureFinished  && isPinch) // allow button under gesture when gesture finished
                {
                    emit scriptProxy->signalGestureFinished(gesture->gestureType());
                }
            }
        }

        return true;
    }

    // dispatches events to QGraphicsItems (and GraphicsSoftButton)
    return QGraphicsView::viewportEvent(event);
}


void GraphicsView::restart()
{
	// clears the screen
	clear();
	// process events
    QTimer::singleShot(0,this,SLOT(populate()));
}

void GraphicsView::onApplicationStateChanged(Qt::ApplicationState state)
{
    switch (state) {
    case Qt::ApplicationActive:
        setPaused(false);
        break;
    case Qt::ApplicationHidden:
    case Qt::ApplicationInactive:
    case Qt::ApplicationSuspended:
        setPaused(true);
        break;
    }
}

void GraphicsView::clear()
{
	timer->stop();

	delete scriptProxy;
    scriptProxy = Q_NULLPTR;

	if(graphicsEngine)
		graphicsEngine->clear(); // clear all item lists

    scene()->clear(); // delete all items

    // high score counter is child of scene
    highScoreCounter_ = Q_NULLPTR;

	hitpointBars_.clear();
	playerVehicles_.clear();

    leftSoftButtons_.clear();
    rightSoftButtons_.clear();
}



void GraphicsView::createHighScoreCounter()
{
    highScoreCounter_ = new QLCDNumber;
    highScoreCounter_->resize(highScoreCounter_->sizeHint() * 3);
    highScoreCounter_->setSegmentStyle(QLCDNumber::Flat);
    highScoreCounter_->setStyleSheet("color: red;"
                                    "background-color: transparent;"
                                             );
    highScoreCounter_->setFrameStyle(QFrame::NoFrame);
    QGraphicsProxyWidget * proxy = scene()->addWidget(highScoreCounter_.data());

    proxy->setZValue(100.0);
    proxy->setPos(0.0 - borderSceneRectDist_.x() +64.0, -1200+32.0);
}

void GraphicsView::createSoftButtons()
{
    GraphicsSoftButton *  shieldButton = new GraphicsSoftButton(":images/ic_filter_tilt_shift_48px.svg");
    GraphicsSoftButton * rotateRightButton = new GraphicsSoftButton(":images/ic_rotate_right_48px.svg");
    GraphicsSoftButton * rotateLeftButton = new GraphicsSoftButton(":images/ic_rotate_left_48px.svg");
    GraphicsSoftButton * shootButton = new GraphicsSoftButton(":images/crosshairs.svg");
    GraphicsSoftButton * accelerateButton = new GraphicsSoftButton(":images/accelerate.svg");

    leftSoftButtons_ = QList<GraphicsSoftButton*>()  << rotateLeftButton << rotateRightButton << shieldButton;
    rightSoftButtons_ = QList<GraphicsSoftButton*>()  <<  shootButton << accelerateButton;
    QList<GraphicsSoftButton*> buttons = QList<GraphicsSoftButton*>() << leftSoftButtons_ << rightSoftButtons_;

    for(GraphicsSoftButton * item : buttons)
    {
        item->setZValue(100.0);
        item->scaleToWidth(256.0);
        scene()->addItem(item);
    }


    connect(accelerateButton,&GraphicsSoftButton::pressed,[this](){
        emit signalKeyPress(Qt::Key_Up);
    });

    connect(accelerateButton,&GraphicsSoftButton::released,[this](){
        emit signalKeyRelease(Qt::Key_Up);
    });

    connect(shieldButton,&GraphicsSoftButton::pressed,[this](){
        emit signalKeyPress(Qt::Key_Delete);
    });
    connect(shieldButton,&GraphicsSoftButton::released, [this](){
        emit signalKeyRelease(Qt::Key_Delete);
    });

    connect(rotateRightButton,&GraphicsSoftButton::pressed,[this](){
        emit signalKeyPress(Qt::Key_Right);
    });
    connect(rotateRightButton,&GraphicsSoftButton::released, [this](){
        emit signalKeyRelease(Qt::Key_Right);
    });

    connect(rotateLeftButton,&GraphicsSoftButton::pressed,[this](){
        emit signalKeyPress(Qt::Key_Left);
    });
    connect(rotateLeftButton,&GraphicsSoftButton::released, [this](){
        emit signalKeyRelease(Qt::Key_Left);
    });

    connect(shootButton,&GraphicsSoftButton::pressed,[this](){
        emit signalKeyPress(Qt::Key_Space);
    });
    connect(shootButton,&GraphicsSoftButton::released, [this](){
        emit signalKeyRelease(Qt::Key_Space);
    });
}

void GraphicsView::populate()
{
    graphicsEngine->gameState()->setPhase(graphicsEngine->gameState()->initialPhase());
	graphicsEngine->setDestroyedAsteroidCount(0);
	setupScript();

	// debug draw
//	QGraphicsRectItem * rect = scene->addRect(scene->sceneRect());
//	rect->setPen(Qt::white);
//	rect->setZValue(-1);

	
    //addPolygons();

	/*
	addRect(QPointF(800.0,-100.0),QSizeF(1600.0,200.0)); // bottom
	addRect(QPointF(100.0,-800.0),QSizeF(200.0,1600.0)); // left
	addRect(QPointF(1500.0,-800.0),QSizeF(200.0,1600.0)); // right
	addRect(QPointF(800.0,-1200.0),QSizeF(1600.0,200.0)); // top
	//*/

	//create polygons that fit the outlines of the spaceships
	// Box2D requires a counterclockwise polygonal chain
	QPolygonF shipPoly;
	shipPoly  <<  QPointF(-25, -35) << QPointF(-41, 0) << QPointF(-25, 35) << QPointF(35, 0);

	// since we can't set the polys inside the script we must retrieve the spaceship-objects from the script
	// and do it here
    for(int i = 0; i< graphicsEngine->gameState()->playerCount(); ++i)
	{
	// object from script
        // TODO use player1 and player2 and put them into the array instead of using qobject list
    QJSValue playerVehicles = scriptProxy->engine()->globalObject().property("playerVehicles");

	// copy script objects into playerVehicles_ array
	playerVehicles_ << qobject_cast<Vehicle*>(playerVehicles.property(i).toQObject());
	playerVehicles_[i]->applyPhysics(graphicsEngine->world(),shipPoly);

	// create a hitpoints bar for each player
	hitpointBars_ << graphicsEngine->createHitpointsBarAt(QPointF(1600.0 + borderSceneRectDist_.x() -128-i*256,-1200+32.0));
	}

    createHighScoreCounter();

#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)

    createSoftButtons();

    adjustSoftButtonPositions();

#endif

	// TimeoutInterval is a global variable and controls the framerate 
    timer->start(TimeoutInterval,this); // starting global timer here
}




void GraphicsView::adjustSoftButtonPositions()
{

    for(int i = 0; i< leftSoftButtons_.size(); ++i)
    {
        GraphicsSoftButton *item = leftSoftButtons_[i];
        item->setPos(QPointF(0.0 - borderSceneRectDist_.x() +128+i*256,-256.0));
    }

    for(int i = 0; i< rightSoftButtons_.size(); ++i)
    {
        GraphicsSoftButton *item = rightSoftButtons_[i];
        item->setPos(QPointF(1600.0 + borderSceneRectDist_.x() -384-i*256,-256.0));
    }
}

void GraphicsView::resizeEvent(QResizeEvent *event)
{
	QGraphicsView::resizeEvent(event);

	// maximumHeight mustn't be higher than sceneRect in normal mode too keep the aspect ratio
	// else aspect ratio errors could happen if the user resized the window manually
	// in fullScreen mode this is handled by Qt::KeepAspectRatioByExpanding as fitInView argument
	qint32 maximumHeight = windowState() == Qt::WindowFullScreen ? QWIDGETSIZE_MAX : width()*1200/1600;
	setMaximumHeight(maximumHeight);

	// adjust the viewport to contain the interesting parts
	if(!this->isFullScreen())
	{
        fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
		this->unsetCursor(); // show cursor
	}
	else
	{
        fitInView(scene()->sceneRect(), Qt::KeepAspectRatioByExpanding);
		this->setCursor(Qt::BlankCursor); // hide cursor
	}
	// borderSceneRectDist_.y() should always be zero

	// get the new window geometry
	int width = event->size().width();
	int height= event->size().height();

	// sceneRect's size is 1600x1200

    // calculate the distance between the window border in scene coordinates and
    //the right border of the graphics scene's scene rect
    // borderSceneRectDist_.x() is used by wrapSprite
	qreal widthSceneCoords = width* (1200.0/height);
	borderSceneRectDist_.setX( (widthSceneCoords -1600)/2.0 );

	// adjust the hitpoints bars on the screen
	for(int i = 0; i< hitpointBars_.size(); ++i)
	{
        hitpointBars_[i]->graphicsProxyWidget()->setPos(1600.0 + borderSceneRectDist_.x() -128-i*256,-(1200.0 - 32.0));
	}
    if(highScoreCounter_ != Q_NULLPTR && highScoreCounter_->graphicsProxyWidget() != Q_NULLPTR)
	{
        highScoreCounter_->graphicsProxyWidget()->setPos(QPointF(0.0 - borderSceneRectDist_.x() +64.0,-(1200.0 - 32.0)));
	}

    adjustSoftButtonPositions();
}


void GraphicsView::closeEvent(QCloseEvent *event)
{
	writeSettings();

    if(highScoreCounter_ != Q_NULLPTR)
    {
        emit signalHighScore(highScoreCounter_->value());
    }
	clear();
	emit signalClose();
	QGraphicsView::closeEvent(event);
}

// keep items inside the visible area
void GraphicsView::wrapSprite(MovingItem * item)
{
	if(!item)
		return;

	// item's coordinate system begins in the top left corner
	qreal itemX = item->x();
	qreal itemY = item->y();

	if ( itemX > 1600.0 +  borderSceneRectDist_.x())
	{
		qreal distance = itemX - (1600.0 +  borderSceneRectDist_.x()); // distance in the "illegal" zone
		item->setPPos( 0.0 - borderSceneRectDist_.x() + distance,item->y());
	}
	else if ( itemX < 0.0f -  borderSceneRectDist_.x())
	{
		qreal distance = (0.0 -  borderSceneRectDist_.x()) - itemX; // distance in the "illegal" zone
		item->setPPos(1600.0f + borderSceneRectDist_.x() - distance,item->y());
	}

   if ( itemY < -scene()->height())
	item->setPPos( item->x(), 0.0f);
   else if ( itemY > 0)
   item->setPPos( item->x(), -scene()->height()+ item->y() );
}

void GraphicsView::timerEvent(QTimerEvent* event)
{

	graphicsEngine->processWorld();
	graphicsEngine->processExplosions();
	graphicsEngine->processProjectiles();
	graphicsEngine->processAsteroids();
	graphicsEngine->processUfos();

	// keep items inside the visible area
    for(Vehicle * asteroid :  graphicsEngine->asteroids())
		wrapSprite(asteroid);

    for(Vehicle * projectile : graphicsEngine->projectiles())
		wrapSprite(projectile);

    for(Vehicle * ufo : graphicsEngine->ufos())
		wrapSprite(ufo);

	if(scriptProxy)
	{
		scriptProxy->timerEvent(event);
		int destroyedPlayerCount = 0;
		// playerVehicles may contain null pointers
		for(int i = 0; i< playerVehicles_.size(); ++i)
		{

			wrapSprite(playerVehicles_[i]);

			if(!playerVehicles_[i])
			{
				++destroyedPlayerCount;
				continue;
			}

			if(playerVehicles_[i]->isVisible() == false ||
				playerVehicles_[i]->indestructible())
				continue;

			if(playerVehicles_[i]->hitpoints() > 1)
			{
                hitpointBars_[i]->setValue(playerVehicles_[i]->hitpoints());
			}
			else
			{
                graphicsEngine->createExplosionAt(playerVehicles_[i]->pos())->setTransform(QTransform::fromScale(0.4, 0.4));
                hitpointBars_[i]->setValue(0);
				delete playerVehicles_[i]; // playerVehicle is automatically set to NULL
				playerVehicles_[i] = NULL;
				graphicsEngine->soundEngine()->stop("loop_engine.wav_" + QString::number(i));
				graphicsEngine->soundEngine()->stop("loop_shield.wav_" + QString::number(i));
				//playerVehicles_.removeAt(i); // dont remove from list, else hitpointBars_ get out of sync
			}
		}

        if(destroyedPlayerCount == playerVehicles_.size()) // set game over to true but dont set it to false when not true
        {
            graphicsEngine->gameState()->setGameOver(true);
        }

        if(destroyedPlayerCount == playerVehicles_.size())
		{
			// if all players are destroyed

            QString message  = tr("Game Over! \n Press ESC to return to menu");;
#ifdef Q_OS_ANDROID
            QString androidMessage = tr("Game Over! \n Press BACK to return to menu");
            message = androidMessage;
#endif
            graphicsEngine->showText(message);

		}
	}
    scene()->advance(); // move items and advance animations

    if(highScoreCounter_ != Q_NULLPTR)
        highScoreCounter_->display(graphicsEngine->destroyedAsteroidCount());
}

void GraphicsView::setPaused(bool b)
{
    if(!timer || !graphicsEngine || b == isPaused())
		return;

        if(isPaused())
		{
		// TimeoutInterval is a global variable
			timer->start(TimeoutInterval,this);
			graphicsEngine->hideText();
		}
        else
		{
			timer->stop();
			graphicsEngine->showText(tr("Pause"));
        }
}

bool GraphicsView::isPaused()
{
    return !timer->isActive();
}

void GraphicsView::toggleFullScreen()
{
	// disable maximumHeight when the window switches to fullscreen mode
	// but enable it in non fullScreen mode keep aspect ratio
	qint32 maximumHeight = windowState() == Qt::WindowFullScreen ? width()*1200/1600 : QWIDGETSIZE_MAX;
	setMaximumHeight(maximumHeight);
	// toggle between fullScreen and normal mode
	this->setWindowState(this->windowState() ^ Qt::WindowFullScreen);
}



void GraphicsView::keyPressEvent(QKeyEvent *event)
{
	if(event->isAutoRepeat())
	{
		event->ignore();
		return;
	}

	switch(event->key())
	{
	case Qt::Key_Pause: // pause game
	{
        setPaused(!isPaused());
		break;
	}
	case Qt::Key_F4:
	{
		restart();
		break;
	}
	case Qt::Key_F11:
	{
		this->toggleFullScreen();
		break;
	}

    #ifdef Q_OS_ANDROID
    case Qt::Key_Back:
    {
        if(graphicsEngine->gameState()->gameOver() || doubleBackToExitPressedOnce_)
        {
           this->close(); // back to menu
        }

        graphicsEngine->showText(tr("Please click BACK again to return to the menu"));
        doubleBackToExitPressedOnce_ = true;
        QTimer::singleShot(2000,this,&GraphicsView::hideDoublePressToExit);

        break;
    }
    #endif
	default:
		{
				emit signalKeyPress(event->key());
		}
	}
}


#ifdef Q_OS_ANDROID
void GraphicsView::hideDoublePressToExit()
{
    graphicsEngine->hideText();
    doubleBackToExitPressedOnce_ = false;
}
#endif

void GraphicsView::keyReleaseEvent(QKeyEvent *event)
{
	if(event->isAutoRepeat())
	{
		event->ignore();
		return;
	}

	if(event->key() == Qt::Key_Escape)
	{
		this->close();
	}
	else
		emit signalKeyRelease(event->key());
}

void GraphicsView::setupScript()
{
    scriptProxy = new JSProxy(this);
	connect(this,SIGNAL(signalKeyPress(int)),scriptProxy,SIGNAL(signalKeyPress(int)));
	connect(this,SIGNAL(signalKeyRelease(int)),scriptProxy,SIGNAL(signalKeyRelease(int)));


    scriptProxy->newQObjectWithName(graphicsEngine->gameState(),"gameState");
    scriptProxy->newQObjectWithName(graphicsEngine,"graphicsEngine");

    scriptProxy->newQObjectWithName(graphicsEngine->soundEngine(),"soundEngine");


	QString srcPath = QApplication::applicationDirPath().remove("bin").append("/src/");
	srcPath = ":";

	if(srcPath != ":")
	{
		//: This is a warning for developers who didn't contain the scripts into the executable
		QMessageBox::warning(this,tr("Not Portable"),tr("Script Files are loaded from the directory ")
		+ srcPath + tr(". The binary won't run anywhere but in the build environment."
		"To Fix this, uncomment srcPath = \":\" in QGraphicsView::setupScript"));
	}

	scriptProxy->evaluateFile( srcPath + "control.js");
	scriptProxy->evaluateFile(srcPath +  "mainloop.js");
}

// read window geometry
void GraphicsView::readSettings()
{
    QSettings settings;
	QString className = this->metaObject()->className();
	this->restoreGeometry(settings.value(className + "/geometry",QByteArray()).toByteArray());
    graphicsEngine->gameState()->setInitialPhase(settings.value("phase",0).toInt());
}

// save window geometry
void GraphicsView::writeSettings()
{
    QSettings settings;
	QString className = this->metaObject()->className();
	settings.setValue(className + "/geometry",saveGeometry());
	if(!settings.contains("phase"))
		settings.setValue("phase",0);
}

int GraphicsView::playerCount() const
{
    return graphicsEngine->gameState()->playerCount();
}

void GraphicsView::setPlayerCount(int playerCount)
{
    return graphicsEngine->gameState()->setPlayerCount(playerCount);
}
