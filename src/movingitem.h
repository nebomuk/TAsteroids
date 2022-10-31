#ifndef MOVINGITEM_H
#define MOVINGITEM_H

#include "animateditem.h"
#include "globalvariables.h"
#include "math.h"
#include "Box2D/Box2D.h"

//  a MovingItem object moves on its own if you set the velocity properties

// A maximumVelocity_ of 0.0,0.0 means the velocity is unbounded

// dont use rotate, _scale and shear, translate cause they will be overriden by setRotation
	// in Qt 4.6 these functions are marked as deprecated

/*
 Movement is completely controlled by the Box2D physics engine.
 after the item has been constructed, you have to call applyPhysics else it will never move
 When the item has been added to a phyics world via applyPhysics  you should not longer use
 setPos directly else the item's position in QGraphicsScene and Box2D will get out of sync.
 instead, use SetPPos, setPX or SetPY
 */


class MovingItem : public AnimatedItem
{
	Q_OBJECT

	Q_PROPERTY(qreal xVelocity READ xVelocity STORED false)
	Q_PROPERTY(qreal yVelocity READ yVelocity STORED false)

	Q_PROPERTY(qreal xAcceleration READ xAcceleration WRITE setXAcceleration STORED false)
	Q_PROPERTY(qreal yAcceleration READ yAcceleration WRITE setYAcceleration STORED false)
	Q_PROPERTY(qreal angularVelocity READ angularVelocity WRITE setAngularVelocity STORED true)
	Q_PROPERTY(qreal angle READ angle WRITE setAngle STORED false)
	Q_PROPERTY(float px READ x WRITE setPX STORED false) // use this property if physics is enabled to set the X pos
    Q_PROPERTY(float py READ y WRITE setPX STORED false)

public:
	MovingItem(QGraphicsItem * parent = 0);
	~MovingItem();

	inline const QPointF& velocity() const {return velocity_;}

	inline qreal xVelocity() const { return velocity_.x();}
	inline qreal yVelocity() const { return velocity_.y();}

	Q_INVOKABLE void setMaximumVelocity(qreal x, qreal y){setMaximumVelocity(QPointF(x,y));}
	void setMaximumVelocity(const QPointF& vel){maximumVelocity_ = vel;}

				inline void setAcceleration(QPointF acceleration){acceleration_= acceleration;}
	Q_INVOKABLE inline void setAcceleration(qreal x, qreal y){setAcceleration(QPointF(x,y));}
	inline const QPointF& acceleration() const {return acceleration_;}

	inline void setXAcceleration(qreal x) { acceleration_.setX(x);}
	inline void setYAcceleration(qreal y) { acceleration_.setY(y);}

	inline qreal xAcceleration() const { return acceleration_.x();}
	inline qreal yAcceleration() const { return acceleration_.y();}

	Q_INVOKABLE inline void applyImpulse(float px, float py);
				inline void applyImpulse(const QPointF& impulse) { applyImpulse(impulse.x(),impulse.y());}

	static void rotateMovingDirection(MovingItem * item);
	Q_INVOKABLE void rotateMovingDirection() { rotateMovingDirection(this);}

	static void flipHorizontal(QGraphicsItem *s);
	Q_INVOKABLE void flipHorizontal(){flipHorizontal(this);}

	// position must be set before this is called!
	void applyPhysics(b2World *world, const QPolygonF &poly); // initializes the b2Body member with a polygon
	void applyPhysics(b2World *world, float radius); // initializes the b2Body with a circle

	// sets the position of the item and the body
	// the item is not updated immediately but afte the next call to advance
	// use setPPos instead of setPos when the item has a body else the coordinate systems get out of sync
	inline void setPPos(float x, float y);
	inline void setPPos(const QPointF& pos){ setPPos(pos.x(),pos.y());}
	inline void setPX(float px){ setPPos(px,y());}
	inline void setPY(float py){ setPPos(x(),py);}


	// compute the angle between x and y speed
	qreal radiansAngleFromVelocity() const {return -atan2(velocity_.x(),velocity_.y());}

	// rate of turn
	inline void setAngularVelocity(qreal angularVelocity);
	qreal angularVelocity() const { return body_? -body_->GetAngularVelocity()*180/PI : 0.0; }

#if QT_VERSION < 0x040600
	// this overrides old transformations. If you want to _scale the item, use _setScale, it will be combined with
	// setRotation
	void _setRotation(qreal angle);
	qreal _rotation() const { return body_ ? -body_->GetAngle()*180/PI : 0.0; }

	void setScale(qreal scale){ scale_ = scale; }
	qreal scale() const { return scale_; }

	// sets the current angle and calls rotate on this item
	void setAngle(qreal angle){_setRotation(angle); }
	qreal angle() const { return _rotation(); }
#else
	void setAngle(qreal angle){setRotation(angle);}
	qreal angle() const { return rotation();}
#endif

	// returns physics body
	b2Body* body() const { return body_; }

	// returns physics world
	b2World* world() const { return world_; }

	enum { Type = UserType+TYPE_MOVINGITEM};
	int type() const {return Type;} // Enable the use of qgraphicsitem_cast with this item.

protected:
	void advance(int phase);
	static inline qreal nonZeroBound(qreal value, qreal max);

	void setupBody(); // used by applyPhysics internally

private:
	QPointF velocity_;
	qreal scale_;
	QPointF  maximumVelocity_;
	QPointF acceleration_;
	QPointF decceleration_;
	b2Body *body_;
	b2World *world_;
};


// returns value bounded by absolute value of max or returns value if max == 0.0
/*static*/ inline qreal MovingItem::nonZeroBound(qreal value, qreal max)
{
	return qFuzzyCompare(max, 0.0) ? value : qBound(-max, value, max);
}

void MovingItem::applyImpulse(float px, float py)
{
	if(body_)
		body_->ApplyImpulse(body_->GetMass()*b2Vec2(px,-py),body_->GetWorldCenter());
}

void MovingItem::setAngularVelocity(qreal angularVelocity)
{
	if(!body_)
		return;
	body_->SetAngularVelocity(-angularVelocity*PI/180.0f);
}

void MovingItem::setPPos(float x, float y)
{
	if(body_)
		body_->SetXForm(b2Vec2(x/SCALE,-y/SCALE),body_->GetAngle());
}

#endif

