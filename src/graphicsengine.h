#ifndef GRAPHICSENGINE_H
#define GRAPHICSENGINE_H

#include <QGraphicsScene>
#include <QPainter>
#include <QGraphicsSimpleTextItem>
#include <QTimer>
#include <QPointer>
#include "Box2D/Box2D.h"
#include "globalvariables.h"

/*
this class provides a convennient interface to the script environment to place enemies in the scene

The createItemAt functions create items at the specified position
BUT drawing and bounding Rect is set to the center ( except for hitpointsBar)
 __________
|		   |
|		   |
|   (xy)   |
|		   |
|__________|

(xy) is the position that is returned by QGraphicsItem::pos()

the processT() functions except for processCollision must be called in e.g. timerEvent
if items should be removed if they collide add them to (max) one of the
lists via one of the appendT functions


this class contains some experimental code that takes the asteroidImages and change their colors
in a seperate thread
*/

class GraphicsEngine : public QObject
{
Q_OBJECT

Q_PROPERTY(int asteroidCount READ asteroidCount STORED false )

public:
enum asteroidType
{
	golevka,
	kleopatra,
	ky26,
	toutatis,
	bacchus
};
	GraphicsEngine(QObject *parent = 0);
	~GraphicsEngine();

	void setScene(QGraphicsScene * scene){scene_ = scene;}

	// must be called in timerEvent
        // TODO only one processEverything function which iterates over every item and decides
        // what action to perform pased on item-types
	void processProjectiles(); // check for collisions and create an explosion if necessary
	void processAsteroids(); // destroy asteroids whose hitpoints == 0
	void processUfos();
	void processExplosions(); // remove explosions that finished its animation

	// advance physics world
	void processWorld();

	b2World * world() const { return world_;} // returns physics world pointer

	SoundEngine * soundEngine() const { return soundEngine_;}

	// returns a list of all submarines tha are handled by the collision detection
	Q_INVOKABLE QList<Vehicle*> asteroids() const { return asteroids_;}
	Q_INVOKABLE QList<Vehicle*> projectiles() const { return projectiles_;}
	Q_INVOKABLE QList<Vehicle*> ufos() const { return ufos_;}


	// returns the number of asteroids that are processed via processAsteroids
	int asteroidCount() { return asteroids_.size();}

	// add a vehicle to projectiles_. processProjectiles() automatically checks for collision for each vehicle in
	// projectiles_
	Q_INVOKABLE inline void appendProjectile(Vehicle* projectile);

	// number of submarines that have been destroyed by projectiles, used for displaying highscore
	Q_INVOKABLE int destroyedAsteroidCount() const { return destroyedAsteroidCount_;}
	void setDestroyedAsteroidCount(int destroyedAsteroidCount){ destroyedAsteroidCount_ = destroyedAsteroidCount; }

	/*Q_INVOKABLE*/ AnimatedItem* createExplosionAt(const QPointF& position);
	/*Q_INVOKABLE*/ AnimatedItem* createHitpointsBarAt(const QPointF& position);

	// makes all colorized (tinted) asteroids available to createAsteroidAt
	// and asynchronously begins to colorize (tint) a copy of all asteroids
	// the index is only a hint which color should be applied next and should be the same as the current level (phase) number
	Q_INVOKABLE void loadColoredAsteroids(int index = 0);

	// clears all lists and stops all sounds
	void clear();

	// create item of type AnimatedItem, MovingItem or Vehicle, initialize it with position and svg and add it to scene
	template<class T>
	T * createTAt(const QPointF& position = QPointF(), const QString& svg = QString());

public slots:

	// these slots create an item at the specified position with the vector graphic svg

	Vehicle * createVehicleAt(const QPointF& position = QPointF(), const QString& svg = QString());
	inline Vehicle *  createVehicleAt(  qreal xPos, qreal yPos, const QString& svg = QString());

	MovingItem * createMovingItemAt(const QPointF& position = QPointF(), const QString& svg = QString());
	inline MovingItem *  createMovingItemAt(  qreal xPos, qreal yPos, const QString& svg = QString());

	AnimatedItem * createAnimatedItemAt(const QPointF& position = QPointF(), const QString& svg = QString());
	inline AnimatedItem *  createAnimatedItemAt(  qreal xPos, qreal yPos,const QString& svg = QString());

	Vehicle * createAsteroidAt(qreal x, qreal y, int asteroidSize = 1);

	Vehicle * createCircleVehicleAt(qreal x, qreal y,float radius = 1.0);

	Vehicle * createUfoAt(qreal x, qreal y);

	// show text for msec miliseconds, by default, text is shown untill hideText is called
	void showText(const QString& text, int msec = 0);

	void hideText() { if(textSprite) textSprite->hide();}

protected:

	void initWorld();

private:
	PixmapList explosionPixmaps_;
	PixmapList hitpointsBarPixmaps_;
	QList<AnimatedItem*> explosions;
	QList<Vehicle*> projectiles_;
	QList<Vehicle*> asteroids_;
	QList<Vehicle*> ufos_;
	ContactListener * contactListener_;
	int destroyedAsteroidCount_;

	AsteroidColorizer * asteroidColorizer_;
	QList<QColor> colorList_;

	b2World * world_; // physics world;

	QGraphicsSimpleTextItem * textSprite;

	QPointer<QGraphicsScene> scene_;
	QTimer hideTextTimer_;

	SoundEngine * soundEngine_;
};

void GraphicsEngine::appendProjectile(Vehicle* projectile)
{
	if(!projectiles_.contains(projectile))
	projectiles_.push_back(projectile);
}

template<class T>
T * GraphicsEngine::createTAt(const QPointF& position, const QString& svg)
{
	T * t = new T;
	t->setFile(svg.isEmpty() ? ":images/redBomb.svg" : svg);
	t->setOffset(T::center(t)); // origin to center
	t->setPos(position);
	scene_->addItem(t);
	return t;
}

Vehicle *  GraphicsEngine::createVehicleAt(  qreal xPos, qreal yPos,const QString& svg)
{
	return  createVehicleAt(QPointF(xPos,yPos),svg);
}

MovingItem *  GraphicsEngine::createMovingItemAt(  qreal xPos, qreal yPos,const QString& svg)

{
	return  createMovingItemAt(QPointF(xPos,yPos),svg);
}

AnimatedItem *  GraphicsEngine::createAnimatedItemAt(  qreal xPos, qreal yPos,const QString& svg)

{
	return  createAnimatedItemAt(QPointF(xPos,yPos),svg);
}

#endif // GRAPHICSENGINE_H
