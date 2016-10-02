#ifndef CONTACTLISTENER_H
#define CONTACTLISTENER_H

#include "vehicle.h"

/*
 This class is used by GraphicsEngine 's Box2D physics world object to handle collisions
	it does decrease hitpoints of players that hit asteroids and decreases hitpoints of
	asteroids that are hit by projectiles
	*/

class ContactListener : public b2ContactListener
	{
	public:
	void Add(const b2ContactPoint* point)
	{
		b2Body* bodys[2];
		bodys[0] = point->shape1->GetBody();
		bodys[1] = point->shape2->GetBody();
		Vehicle * vehicles[2];
		QGraphicsItem * items[2];
		// GetUserData returns a void* in which we stored a QGraphicsItem* before
		// its completely safe to cast it back to its original type
		items[0] = static_cast<QGraphicsItem*>(bodys[0]->GetUserData());
		items[1] = static_cast<QGraphicsItem*>(bodys[1]->GetUserData());
		// the QGraphicsItem* actually is a pointer to a subclass, we try to downcast it
		// if qgraphicsitem_cast fails, it returns 0 and the next if triggers
		vehicles[0] = qgraphicsitem_cast<Vehicle*>(items[0]);
		vehicles[1] = qgraphicsitem_cast<Vehicle*>(items[1]);

		if(!vehicles[0] || !vehicles[1])
			return;

		if(vehicles[0]->isPlayer() &&  vehicles[1]->isAsteroid())
			vehicles[0]->setHitpoints(vehicles[0]->hitpoints()-1);

		else if(vehicles[1]->isPlayer() &&  vehicles[0]->isAsteroid())
			vehicles[1]->setHitpoints(vehicles[1]->hitpoints()-1);

		else if((vehicles[0]->isProjectile() ||  vehicles[1]->isProjectile())
			&& vehicles[0]->diplomacy() != vehicles[1]->diplomacy())
			for(int i = 0; i <2; ++i)
				vehicles[i]->setHitpoints(vehicles[i]->hitpoints()-1);

	}
	};

#endif // CONTACTLISTENER_H
