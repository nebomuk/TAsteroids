#include "graphicsengine.h"
#include <QFont>
#include <QBitmap>
#include <QProgressBar>
#include <QGraphicsProxyWidget>
#include "svgcache.h"
#include "asteroidcolorizer.h"
#include "contactlistener.h"
#include "vehicle.h"
#include "soundengine.h"
#include "gamestate.h"

GraphicsEngine::GraphicsEngine(QObject *parent) :
	QObject(parent)
{
	textSprite = NULL;
	world_ = NULL;

	initWorld();

	asteroidColorizer_ = new AsteroidColorizer();

    colorList_ << QColor(120,0,0) << QColor(0,0,150) << QColor(0,100,0)
               << QColor(150,150,0)	<< QColor(150,0,150) << QColor(150,50,0) << QColor(150,0,50) << QColor(0,150,50)
             << QColor(0,150,150) << QColor(0,75,150);

    QImage first = QImage(":images/explosion/explosionBackground.png");
	int explosionWidth = first.width();
    explosionImages_ << first.scaledToWidth(qRound(explosionWidth*1.5f),Qt::SmoothTransformation);
	for(int i = 0; i <15; ++i)
	{
        QImage image(":images/explosion/explosion" + QString::number(i)+".png");
        explosionImages_ << image.scaledToWidth(qRound(explosionWidth*1.5f),Qt::SmoothTransformation);
	}

	QList<QSvgRenderer*> renderers;
	for(int i = 0; i<11; ++i)
		renderers.push_back(new QSvgRenderer(":hitpointsBar_images/hitpointsBar" + QString::number(i) + ".svg"));

	SvgCache * hitpointsBarCache = new SvgCache;
	hitpointsBarCache->addSvgRenderers(renderers);
    hitpointsBarImages_ << hitpointsBarCache->images();

	hideTextTimer_.setSingleShot(true);
	connect(&hideTextTimer_,SIGNAL(timeout()),this,SLOT(hideText()));

	destroyedAsteroidCount_ = 0;

    gameState_ = new GameState(this);
	soundEngine_ = new SoundEngine(this);
	QStringList soundFiles = QDir(":sounds/").entryList();
	for(int i = 0; i< soundFiles.size(); ++i)
		soundFiles[i].prepend(":sounds/");

	soundEngine_->loadSounds(soundFiles);
}

GraphicsEngine::~GraphicsEngine()
{
	delete contactListener_;
	delete world_;
	delete asteroidColorizer_;
}

void GraphicsEngine::initWorld()
{
	// Define world, gravity
	b2AABB worldAABB;
	float adjust = 400.0f;
	worldAABB.lowerBound.Set(0.0f-adjust, 0.0f-adjust);
	//worldAABB.upperBound.Set(1600.0f/SCALE+adjust, 1200.0f/SCALE+adjust);
	worldAABB.upperBound.Set(1600.0f/SCALE+adjust, 1200.0f/SCALE+adjust); // 4 times too big
	world_ = new b2World(worldAABB,
						/* gravity = */ b2Vec2(0.0f, -0.0f),
						/* doSleep = */ true); // doSleep allows bodies to sleep if they come to rest
	contactListener_ = new ContactListener;
	world_->SetContactListener(contactListener_);
}

GameState *GraphicsEngine::gameState() const
{
    return gameState_;
}


void GraphicsEngine::processExplosions()
{
    for(int i = 0; i < explosions.size(); ++i)
	{
		AnimatedItem * explosion = explosions[i];

		if(explosion->loopsRemaining() == 0) // animation finished
		{ // remove
			explosions.removeAt(i);
			scene_->removeItem(explosion);
			delete explosion;
		}
	}
}

void  GraphicsEngine::processProjectiles()
{
	for(int i = 0; i < projectiles_.size(); ++i)
	{
		Vehicle * projectile = projectiles_[i];

		if(!projectile)
		{
			projectiles_.removeAt(i);
			continue;
		}

		// grow older
		projectile->setAge(projectile->age()+1);

		if(projectile->hitpoints() <= 0	|| projectile->age() == 30)
		{
				projectiles_.removeAt(i);
				scene_->removeItem(projectile);
				delete projectile;
		}
	}
}

void GraphicsEngine::processAsteroids()
{
	for(int i = 0; i < asteroids_.size(); ++i)
	{
		Vehicle * asteroid = asteroids_[i];

		if(!asteroid )
		{
			asteroids_.removeAt(i);
			continue;
		}

		if(asteroid->hitpoints() <= 0)
		{
			++destroyedAsteroidCount_;

			int asteroidSize = asteroid->size();
			if(asteroidSize != 0) // small asteroids vanish
			{
				// medium and big asteroids split into smaller asteroids
				for(int i = 0; i <4 ; ++i)
					createAsteroidAt(asteroid->x(),asteroid->y(),asteroidSize-1);
			}
                        qreal factor = asteroid->scale();
                        createExplosionAt(asteroid->pos())->setTransform(QTransform::fromScale(factor,factor));
			asteroids_.removeAt(i);
			scene_->removeItem(asteroid);
			delete asteroid;
		}
	}
}

void GraphicsEngine::processUfos()
{
	for(int i = 0; i < ufos_.size(); ++i)
	{
		Vehicle * ufo = ufos_[i];

		if(!ufo)
		{
			ufos_.removeAt(i);
			continue;
		}
		if(ufo->hitpoints() <= 0)
        {
                        AnimatedItem * explosion = createExplosionAt(ufo->pos());
                        explosion->setTransform(QTransform::fromScale(0.7,0.7));
        }

        if(ufo->hitpoints() <= 0 || ufo->wormholeState() == Vehicle::INSIDE)
		{
			++destroyedAsteroidCount_;
			ufos_.removeAt(i);
			scene_->removeItem(ufo);
			delete ufo;
		}
	}
}

void GraphicsEngine::processWorld()
{
	world_->Step(B2_TIMESTEP, B2_ITERATIONS);
}

void GraphicsEngine::showText( const QString &text, int msec /* = 0 */)
{
    if(text == activeText_ && msec == 0)
        return;

    activeText_ = text;
	if(!textSprite)
	{
		textSprite = new QGraphicsSimpleTextItem;
		QFont font( "OCR A Extended", 32);
		textSprite->setFont( font );
		textSprite->setZValue(20.0);
		textSprite->setBrush(Qt::white);
	}

	if(hideTextTimer_.isActive())
		hideTextTimer_.stop();

	if(msec > 0)
	{
		hideTextTimer_.setInterval(msec);
		hideTextTimer_.start();
	}

	if(!textSprite->scene())
		scene_->addItem(textSprite);

	textSprite->setText(text);

	// center
	textSprite->setPos( (scene_->width()-textSprite->boundingRect().width()) / 2,
							-(scene_->height()-textSprite->boundingRect().height()) / 2 );

	textSprite->show();
}

void GraphicsEngine::hideText() {

    activeText_.clear();
    if(textSprite)
    {
        textSprite->hide();
    }
}

void GraphicsEngine::clear()
{
    textSprite = NULL;
    explosions.clear();
	projectiles_.clear();
	asteroids_.clear();
	ufos_.clear();
	soundEngine_->stopAll();
}

AnimatedItem* GraphicsEngine::createExplosionAt(const QPointF& position)
{
	AnimatedItem * explosion = new AnimatedItem;
    explosion->addImages(explosionImages_);
	explosion->setZValue(5.0);
	explosion->setFrameRateDivisor(2);
	explosion->setPos(position);
	explosion->setOffset(AnimatedItem::center(explosion));
	explosion->setLoopCount(1);
	explosions.push_back(explosion);
	scene_->addItem(explosion);
	soundEngine_->play("explosion.wav");
	return explosion;
}

QProgressBar* GraphicsEngine::createHitpointsBarAt(const QPointF& position)
{
    QProgressBar * progress = new QProgressBar();
    progress->resize(100,50);
    progress->setRange(0,10);
    progress->setValue(5);
    progress->setTextVisible(false);
    progress->setStyleSheet("QProgressBar{"
                               "border: 0px solid transparent;"
                               "color:rgba(0,0,0,100);"
                               "border-radius: 4px;"
                               "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(80, 160, 160, 100), stop:1 rgba(200, 200, 200, 100));"
                                   "}"
                               "QProgressBar::chunk{"
                               "background-color: rgba(0,255,0,100);"
                               "}");
    QGraphicsProxyWidget * proxy =  scene_->addWidget(progress);
    proxy->setZValue(100.0);
    proxy->setPos(position);
    return progress;
}

Vehicle * GraphicsEngine::createVehicleAt(const QPointF& position /*=QPointF()*/,
									 const QString& svg /* = QString() */)
{
    Vehicle * vehicle = createTAt<Vehicle>(position,svg);
    vehicle->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    return vehicle;
}

Vehicle * GraphicsEngine::createCircleVehicleAt(qreal x, qreal y, float radius)
{
	Vehicle * vehicle = new Vehicle;
	vehicle->setPos(x,y);
	vehicle->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
	vehicle->setZValue(3.0);
	vehicle->setFile(":images/redBomb.svg");
	vehicle->applyPhysics(world_,radius);
	vehicle->setOffset(AnimatedItem::center(vehicle)); // origin to center
	scene_->addItem(vehicle);
	return vehicle;
}

MovingItem * GraphicsEngine::createMovingItemAt(const QPointF& position,
								   const QString& svg)
{
		return createTAt<MovingItem>(position,svg);
}

AnimatedItem * GraphicsEngine::createAnimatedItemAt(const QPointF& position,
								   const QString& svg)
{
		return createTAt<AnimatedItem>(position,svg);
}

Vehicle * GraphicsEngine::createAsteroidAt(qreal x, qreal y, int asteroidSize)
{
	qreal scale = 1.0;

	switch(asteroidSize)
	{
    case 2: scale = 2.0; break;
    case 1: scale = 1.0; break;
    case 0: scale = 0.5; break;
	default:;
	}

	Vehicle * asteroid = new Vehicle;
	asteroid->setAsteroid(true);

	asteroidType type = static_cast<asteroidType>(qrand() % 4);

    QColor tintColor;
    if(gameState_->phase() > 0)
    {
        tintColor = colorList_.at(gameState()->phase() % (colorList_.size()-1));
    }
	switch(type)
	{
    case golevka:	asteroid->addImages(asteroidColorizer_->imagesForAsteroid("golevka",tintColor)); break;
    case kleopatra: asteroid->addImages(asteroidColorizer_->imagesForAsteroid("kleopatra",tintColor)); break;
    case ky26:		asteroid->addImages(asteroidColorizer_->imagesForAsteroid("ky26",tintColor)); break;
    case toutatis:  asteroid->addImages(asteroidColorizer_->imagesForAsteroid("toutatis",tintColor)); break;
	default:
		qDebug("GraphicsEngine::createAsteroidAt: invalid asteroidType");
        return Q_NULLPTR;
	}
	int startFrame = qrand() % 32;
	asteroid->setFrame(startFrame);
	asteroid->setFrameRateDivisor(3);
	asteroid->setZValue(3.0);
	asteroid->setOffset(Vehicle::center(asteroid)); // origin to center
	asteroid->setPos(x,y);
	asteroid->setDiplomacy(2);

	asteroid->setScale(scale);
	asteroid->setSize(asteroidSize);

    QSize size = asteroid->imageAt(0).size()*scale;
	// compute average side length

	float side = (size.width() + size.height()) / 2.0f;
	asteroid->applyPhysics(world_,side/2.0f);
	qreal randNum0 = (qrand() % 2000 - 1000) / 100.0f;
	qreal randNum1 = (qrand() % 2000 - 1000) / 100.0f;
	asteroid->applyImpulse(randNum0*1/scale,randNum1*1/scale); // random speed, smaller asteroids are faster
	asteroids_ << asteroid;
	scene_->addItem(asteroid);
	return asteroid;
}

Vehicle * GraphicsEngine::createUfoAt(qreal x, qreal y)
{
	Vehicle * ufo = createTAt<Vehicle>(QPointF(x,y),":ufo.svg");
	ufo->setDiplomacy(3);
	ufo->setMaximumVelocity(20,20);
	QPolygonF ufoPoly;
	ufoPoly << QPointF(-45,0) << QPointF(-14,30) << QPointF(40,45) << QPointF(45,0) << QPointF(40,-45) << QPointF(-14,-30);
	ufo->applyPhysics(world_,ufoPoly);
	ufos_ << ufo;
	return ufo;
}

