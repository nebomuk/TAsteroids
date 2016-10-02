#ifndef VEHICLE_H
#define VEHICLE_H

#include <QPainter>
#include "movingitem.h"
#include <cmath>

// if you set the velocity properties inherited from MovingItem, a Vehicle object will move automatically

// if you want to compare types always use e.g.  myItem->type() == QGraphicsItem::UserType +TYPE_ITEMTYPE

// call beginWormholeTravel or endWormholeTravel, those functions will automatically set the correct state
// wormholeFrame 0 : no effect, outside, wormholeFrame 255 : invisible, inside
// you can't decrease the hitpoints while the vehicle is not in state outside and while the vehicle is indestructible

class Vehicle : public MovingItem
{
	Q_OBJECT

	Q_ENUMS (WormholeState)
	Q_PROPERTY(int hitpoints READ hitpoints WRITE setHitpoints)
	Q_PROPERTY(bool indestructible READ indestructible WRITE setIndestructible)
	Q_PROPERTY(int diplomacy READ diplomacy WRITE setDiplomacy)
	Q_PROPERTY(int age READ age WRITE setAge)
	Q_PROPERTY(int wormholeFrame READ wormholeFrame WRITE setWormholeFrame)
	Q_PROPERTY(WormholeState wormholeState READ wormholeState)
	Q_PROPERTY(bool isPlayer READ isPlayer WRITE setPlayer)
	Q_PROPERTY(bool isProjectile READ isProjectile WRITE setProjectile)
	Q_PROPERTY(bool isAsteroid READ isAsteroid WRITE setAsteroid)

public:
	enum { Type = UserType+TYPE_VEHICLE};
	int type() const {return Type;} // Enable the use of qgraphicsitem_cast with this item.

	enum WormholeState {outside, vanish, inside, appear};

	Vehicle(QGraphicsItem * parent = 0);
	~Vehicle() { delete [] shape;}

	void setHitpoints(int hitpoints);
	int hitpoints() const { return hitpoints_;}

	// if this is set, all calls to setHitpoints can only increase hitpoints_
	void setIndestructible(bool b){indestructible_ = b;}
	bool indestructible() const { return indestructible_;}

	// important for collision detection. by default, diplomacy is neutral == 0
	void setDiplomacy(int diplomacy){	diplomacy_ = diplomacy;}
	int diplomacy() const { return diplomacy_;}

	// used by collision detection
	void setPlayer(bool isPlayer){ isPlayer_ = isPlayer;}
	bool isPlayer() const { return isPlayer_; }

	// used by collision detection
	void setProjectile(bool isProjectile){ isProjectile_ = isProjectile;}
	bool isProjectile() const { return isProjectile_; }

	void setAsteroid(bool isAsteroid){ isAsteroid_ = isAsteroid; }
	bool isAsteroid() const { return isAsteroid_; }

	// allows the graphics engine to remove this item if this item is too "old"
	void setAge(int age){ age_ = age; }
	int age() const { return age_; }

	// used by the graphics engine
	void setSize(int size){ size_ = size; }
	int size() const { return size_; }

	// frame goes from 0..255
	void setWormholeFrame(int frame);
	int wormholeFrame() const { return wormholeFrame_;}

	WormholeState wormholeState() const { return wormholeState_; }

	// sets wormholeTravelDirection_ to 1/-1 and disables/enables collisionDetection
	Q_INVOKABLE void beginWormholeTravel();
	Q_INVOKABLE void endWormholeTravel();

protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
	void advance(int phase);

	// called by setFile
	virtual void fileChanged();

	// prepare the tables which define the wormhole effect shape
	void setupWormhole();

	// frame goes from 0..255
	void animate(int frame);

private:
	int hitpoints_;
	bool indestructible_;
	int diplomacy_;
	int age_;
	int size_;
	QImage sourceImage;
	QImage effectImage;
	int *shape;
	int wormholeFrame_;
	WormholeState wormholeState_;
	bool isProjectile_;
	bool isPlayer_;
	bool isAsteroid_;
};
#endif
