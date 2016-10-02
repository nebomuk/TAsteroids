#include "graphicsengine.h"
#include <QFont>
#include <QBitmap>
#include "svgcache.h"
#include "asteroidcolorizer.h"
#include "contactlistener.h"
#include "vehicle.h"
#include "soundengine.h"

GraphicsEngine::GraphicsEngine(QObject *parent) :
	QObject(parent)
{
	textSprite = NULL;
	world_ = NULL;

	initWorld();

	asteroidColorizer_ = new AsteroidColorizer();

	colorList_ << QColor(150,0,0) << QColor(0,0,150) << QColor(0,150,0)
			<< QColor(50,0,150) << QColor(50,150,0)  << QColor(0,50,150)
			<< QColor(150,50,0) << QColor(150,0,50) << QColor(0,150,50)
			<< QColor(150,150,0) << QColor(0,150,150) << QColor(150,0,150);

    QImage first = QImage(":explosion_images/explosionBackground.png");
	int explosionWidth = first.width();
    explosionImages_ << first.scaledToWidth(explosionWidth*3,Qt::SmoothTransformation);
	for(int i = 0; i <15; ++i)
	{
        QImage image(":explosion_images/explosion" + QString::number(i)+".png");
        explosionImages_ << image.scaledToWidth(explosionWidth*3,Qt::SmoothTransformation);
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
                        createExplosionAt(ufo->pos())->setTransform(QTransform::fromScale(0.4,0.4));

		if(ufo->hitpoints() <= 0 || ufo->wormholeState() == Vehicle::inside)
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

AnimatedItem* GraphicsEngine::createHitpointsBarAt(const QPointF& position)
{
	AnimatedItem * hitpointsBar = new AnimatedItem;
	hitpointsBar->setAdvancing(false);
    hitpointsBar->addImages(hitpointsBarImages_);
	hitpointsBar->setZValue(100.0);
	//hitpointsBar->setOffset(AnimatedItem::center(hitpointsBar));
	hitpointsBar->setPos(position);
	hitpointsBar->setFrame(10);
	scene_->addItem(hitpointsBar);
	return hitpointsBar;
}

Vehicle * GraphicsEngine::createVehicleAt(const QPointF& position /*=QPointF()*/,
									 const QString& svg /* = QString() */)
{
		return createTAt<Vehicle>(position,svg);
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
	case 2: scale = 1.0; break;
	case 1: scale = 0.5; break;
	case 0: scale = 0.25; break;
	default:;
	}

	Vehicle * asteroid = new Vehicle;
	asteroid->setAsteroid(true);

	asteroidType type = static_cast<asteroidType>(qrand() % 4);

	switch(type)
	{
    case golevka:	asteroid->addImages(asteroidColorizer_->imagesForAsteroid("golevka")); break;
    case kleopatra: asteroid->addImages(asteroidColorizer_->imagesForAsteroid("kleopatra")); break;
    case ky26:		asteroid->addImages(asteroidColorizer_->imagesForAsteroid("ky26")); break;
    case toutatis:  asteroid->addImages(asteroidColorizer_->imagesForAsteroid("toutatis")); break;
	default:
		qDebug("GraphicsEngine::createAsteroidAt: invalid asteroidType");
		return NULL;
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

void GraphicsEngine::loadColoredAsteroids(int index)
{
	if(!asteroidColorizer_ || index < 0)
		return;

	if(index == 0)
		asteroidColorizer_->loadDefault();

	asteroidColorizer_->imagesIntoPixmaps();

	asteroidColorizer_->startConcurrentTint(colorList_[index%colorList_.size()]);
}
