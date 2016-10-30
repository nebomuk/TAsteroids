#include <QDesktopServices>
#include <QApplication>
#include <QSettings>
#include <QDir>
#include <QScriptValue>
#include <QMessageBox>
#include <QEvent>
#include <QGestureEvent>
#include "graphicsview.h"
#include "globalvariables.h"
#include "vehicle.h"
#include "svgcache.h"
#include "scriptproxy.h"
#include "graphicsengine.h"
#include "mechanicalcounter.h"
#include "soundengine.h"
#include "graphicssoftbutton.h"
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
	lastHighScore_ = 0;

	this->setWindowTitle("TAsteroids");

	timer = new QBasicTimer;

	// dataLocation is the location for this application's data
#ifdef Q_OS_LINUX
	dataLocation = QString(QDir::homePath() + "/.tasteroids/");
#else
	dataLocation = QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/tasteroids/";
#endif





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
    gestures << Qt::TapGesture << Qt::TapAndHoldGesture;
    foreach(Qt::GestureType gesture, gestures)
    {
         this->viewport()->grabGesture(gesture);
    }
    viewport()->setAttribute(Qt::WA_AcceptTouchEvents);

	this->setRenderHint(QPainter::Antialiasing,false);

	setStyleSheet("QGraphicsView { border: none }");

	scene = new QGraphicsScene(0.0,-1200.0,1600.0,1200.0,this);
	scene->setItemIndexMethod(QGraphicsScene::NoIndex);
	scene->setBackgroundBrush(QPixmap(":images/stars1.png"));
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
        //QGesture* gesture = e->gesture(Qt::TapGesture);
        QList<QGesture*> gestures = e->gestures();
        foreach(QGesture* gesture, gestures)
        {
            Qt::GestureType type = gesture->gestureType();
            if(type == Qt::TapGesture)
            {
                if(gesture->state() == Qt::GestureStarted)
                {
                    qDebug() << "Tap Gesture started";
                    emit signalKeyPress(Qt::Key_Space);
                }
                else if(gesture->state() == Qt::GestureFinished)
                {
                    qDebug() << "Tap Gesture stopped";
                    emit signalKeyRelease(Qt::Key_Space);
                }
            }
//            else if(type == Qt::TapAndHoldGesture)
//            {
//                if(gesture->state() == Qt::GestureStarted)
//                {
//                    qDebug() << "Tap and hold Gesture started";
//                    emit signalKeyPress(Qt::Key_Up);
//                }
//                else if(gesture->state() == Qt::GestureFinished)
//                {
//                    qDebug() << "Tap and hold Gesture stopped";
//                    emit signalKeyRelease(Qt::Key_Up);
//                }
//            }
        }

        return true;
    }
//    else if (event->type() == QEvent::TouchBegin)
//    {
//      return false;
//    }
    else return QGraphicsView::viewportEvent(event);
}

void GraphicsView::restart()
{
	// clears the screen
	clear();
	// process events
	QTimer::singleShot(0,this,SLOT(populate()));
}

void GraphicsView::clear()
{
	timer->stop();

	if(highScoreCounter_)
		lastHighScore_ = highScoreCounter_->value();

	delete scriptProxy;
    scriptProxy = Q_NULLPTR;

	foreach(b2Body* body, groundBodies_)
		graphicsEngine->world()->DestroyBody(body);

		// destroy side rects
	groundBodies_.clear();

	if(graphicsEngine)
		graphicsEngine->clear(); // clear all item lists

	scene->clear(); // delete all items

	bodyItems_.clear();

    //delete highScoreCounter_; // crashes
    highScoreCounter_ = Q_NULLPTR;

	hitpointBars_.clear();
	playerVehicles_.clear();
}

// for testing purposes only, adds a physics aware rectangle
QGraphicsRectItem * GraphicsView::addRect(QPointF pos, QSizeF size)
{
	b2BodyDef bodyDef;
	b2PolygonDef shapeDef;
	QGraphicsRectItem *groundItem = scene->addRect(-size.width()/2, -size.height()/2,size.width(),size.height());
	groundItem->setBrush(QColor(178, 192, 160));
	groundItem->setPos(pos);
	groundItem->setPen(Qt::NoPen);
	bodyDef.position.Set(pos.x()/SCALE, -pos.y()/SCALE);
	shapeDef.SetAsBox(size.width()/2/SCALE, size.height()/2/SCALE);
	b2Body * groundBody = graphicsEngine->world()->CreateBody(&bodyDef);
	groundBody->SetUserData(static_cast<void*>(groundItem));
	groundBody->CreateShape(&shapeDef);
	groundBodies_ << groundBody;
	return groundItem;
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

	
	// create some colored fun polygons that you can push around
	QPolygonF poly;
	poly << QPointF(0, -10)*5 << QPointF(-5, 0)*5 << QPointF(5, 0)*5;
	for (int i = 0; i < 3; ++i) {
		QGraphicsBox2DPolygonItem *polygon = new QGraphicsBox2DPolygonItem(graphicsEngine->world());
		polygon->setPos(600, -400);
		polygon->setRotation(qrand() % 360);
		polygon->setPolygon(poly);
		polygon->setBrush(QColor(128 + qrand() % 128, 128 + qrand() % 128, 128 + qrand() % 128));
		polygon->setup();
		bodyItems_ << polygon;
		scene->addItem(polygon);
	}

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
	QScriptValue playerVehicles = scriptProxy->engine()->globalObject().property("playerVehicles");

	// copy script objects into playerVehicles_ array
	playerVehicles_ << qobject_cast<Vehicle*>(playerVehicles.property(i).toQObject());
	playerVehicles_[i]->applyPhysics(graphicsEngine->world(),shipPoly);

	// create a hitpoints bar for each player
	hitpointBars_ << graphicsEngine->createHitpointsBarAt(QPointF(1600.0 + borderSceneRectDist_.x() -128-i*256,-1200+32.0));
	}

	// a counter that shows the highscore
	highScoreCounter_ = new MechanicalCounter;
	highScoreCounter_->setDigitsFile(":train_digits.svg");
	highScoreCounter_->setZValue(100.0);
	highScoreCounter_->setPos(0.0 - borderSceneRectDist_.x() +64.0, -1200+32.0);

	scene->addItem(highScoreCounter_);


#ifdef Q_OS_ANDROID
    QList<GraphicsSoftButton*> buttons;
    buttons << new GraphicsSoftButton(":images/ic_filter_tilt_shift_48px.svg");
    buttons << new GraphicsSoftButton(":images/ic_rotate_right_48px.svg");
    buttons << new GraphicsSoftButton(":images/ic_rotate_left_48px.svg");
    for(int i = 0; i< buttons.size(); ++i)
    {
        GraphicsSoftButton * item = buttons[i];
        item->setZValue(100.0);
        item->setPos(QPointF(1600.0 + borderSceneRectDist_.x() -128-i*256,-128.0));
        item->scaleToWidth(128.0);
        scene->addItem(item);
        softButtons_ << item;
    }
#endif




	// TimeoutInterval is a global variable and controls the framerate 
	timer->start(TimeoutInterval,this); // starting global timer here
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
		fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
		this->unsetCursor(); // show cursor
	}
	else
	{
		fitInView(scene->sceneRect(), Qt::KeepAspectRatioByExpanding);
		this->setCursor(Qt::BlankCursor); // hide cursor
	}
	// borderSceneRectDist_.y() should always be zero

	// get the new window geometry
	int width = event->size().width();
	int height= event->size().height();

	// sceneRect's size is 1600x1200

	// calculate the distance between the border in scene coordinates and the right border scene
	// because borderSceneRectDist_.x() is used by wrapSprite
	qreal widthSceneCoords = width* (1200.0/height);
	borderSceneRectDist_.setX( (widthSceneCoords -1600)/2.0 );

	// adjust the hitpoints bars on the screen
	for(int i = 0; i< hitpointBars_.size(); ++i)
	{
		hitpointBars_[i]->setPos(1600.0 + borderSceneRectDist_.x() -128-i*256,-(1200.0 - 32.0));
	}
	if(highScoreCounter_)
	{
		highScoreCounter_->setPos(QPointF(0.0 - borderSceneRectDist_.x() +64.0,-(1200.0 - 32.0)));
	}
}

void GraphicsView::closeEvent(QCloseEvent *event)
{
	writeSettings();
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

   if ( itemY < -scene->height())
	item->setPPos( item->x(), 0.0f);
   else if ( itemY > 0)
   item->setPPos( item->x(), -scene->height()+ item->y() );
}

void GraphicsView::timerEvent(QTimerEvent* event)
{

	graphicsEngine->processWorld();
	foreach(QGraphicsBox2DPolygonItem* item, bodyItems_)
		item->adjust(); // apply physics calculations

	graphicsEngine->processExplosions();
	graphicsEngine->processProjectiles();
	graphicsEngine->processAsteroids();
	graphicsEngine->processUfos();

	// keep items inside the visible area
	foreach(Vehicle * asteroid, graphicsEngine->asteroids())
		wrapSprite(asteroid);

	foreach(Vehicle * projectile, graphicsEngine->projectiles())
		wrapSprite(projectile);

	foreach(Vehicle * ufo, graphicsEngine->ufos())
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
				hitpointBars_[i]->setFrame(playerVehicles_[i]->hitpoints());
			}
			else
			{
                graphicsEngine->createExplosionAt(playerVehicles_[i]->pos())->setTransform(QTransform::fromScale(0.4, 0.4));
				hitpointBars_[i]->setFrame(0);
				delete playerVehicles_[i]; // playerVehicle is automatically set to NULL
				playerVehicles_[i] = NULL;
				graphicsEngine->soundEngine()->stop("loop_engine.wav_" + QString::number(i));
				graphicsEngine->soundEngine()->stop("loop_shield.wav_" + QString::number(i));
				//playerVehicles_.removeAt(i); // dont remove from list, else hitpointBars_ get out of sync
			}
		}

		if(destroyedPlayerCount == playerVehicles_.size())
		{
			// if all players are destroyed
			graphicsEngine->showText("Game Over! \n Press ESC to return to menu");
		}
	}
	scene->advance(); // move items and advance animations

	if(highScoreCounter_ && highScoreCounter_->value() != graphicsEngine->destroyedAsteroidCount())
		highScoreCounter_->setValue(graphicsEngine->destroyedAsteroidCount());
}

void GraphicsView::togglePause()
{
	if(!timer || !graphicsEngine)
		return;

		if(!timer->isActive())
		{
		// TimeoutInterval is a global variable
			timer->start(TimeoutInterval,this);
			graphicsEngine->hideText();
		}
		else if(timer->isActive())
		{
			timer->stop();
			graphicsEngine->showText(tr("Pause"));
		}
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
		togglePause();
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
	default:
		{
				emit signalKeyPress(event->key());
		}
	}
}

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
	scriptProxy = new ScriptProxy(this);
	connect(this,SIGNAL(signalKeyPress(int)),scriptProxy,SIGNAL(signalKeyPress(int)));
	connect(this,SIGNAL(signalKeyRelease(int)),scriptProxy,SIGNAL(signalKeyRelease(int)));


    scriptProxy->newQObjectByName(graphicsEngine->gameState(),"gameState", QScriptEngine::QtOwnership,
									QScriptEngine::ExcludeSuperClassProperties |
									QScriptEngine::ExcludeSuperClassMethods);
	scriptProxy->newQObjectByName(graphicsEngine,"graphicsEngine", QScriptEngine::QtOwnership,
									QScriptEngine::ExcludeSuperClassProperties |
									QScriptEngine::ExcludeSuperClassMethods);

	scriptProxy->newQObjectByName(graphicsEngine->soundEngine(),"soundEngine", QScriptEngine::QtOwnership,
									QScriptEngine::ExcludeSuperClassProperties |
									QScriptEngine::ExcludeSuperClassMethods);


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

int GraphicsView::lastHighScore() const
{
	return lastHighScore_;
}

// read window geometry
void GraphicsView::readSettings()
{
	QSettings settings(dataLocation + "tasteroids.ini", QSettings::IniFormat,this);
	QString className = this->metaObject()->className();
	this->restoreGeometry(settings.value(className + "/geometry",QByteArray()).toByteArray());
    graphicsEngine->gameState()->setInitialPhase(settings.value("phase",0).toInt());
}

// save window geometry
void GraphicsView::writeSettings()
{
	QSettings settings(dataLocation + "tasteroids.ini", QSettings::IniFormat,this);
	QString className = this->metaObject()->className();
	settings.setValue(className + "/geometry",saveGeometry());
	if(!settings.contains("phase"))
		settings.setValue("phase",0);
}
