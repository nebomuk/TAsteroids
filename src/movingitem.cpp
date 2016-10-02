#include "movingitem.h"
#include "signum.h"

MovingItem::MovingItem(QGraphicsItem * parent)
	:
	AnimatedItem(parent),
	velocity_(0.0,0.0),
	scale_(1.0),
	maximumVelocity_(0.0,0.0),
	acceleration_(0.0,0.0),
	decceleration_(0.0,0.0),
	body_(NULL),
	world_(NULL)
{
}

MovingItem::~MovingItem()
{
	if(world_)
		world_->DestroyBody(body_);
}

#if QT_VERSION < 0x040600
void MovingItem::_setRotation(qreal angle)
{
	qreal radAngle = angle * PI/180.0;
	QTransform rotation(cos(radAngle), sin(radAngle), -sin(radAngle), cos(radAngle), 0, 0);
	setTransform(rotation*scale_);
}
#endif

/*static*/ void MovingItem::flipHorizontal(QGraphicsItem *s)
{
        s->setTransform(QTransform::fromScale(-1, 1));
        //s->translate(-s->boundingRect().width(),0);
}

/*static*/ void MovingItem::rotateMovingDirection(MovingItem * item)
{
			   qreal angle = 0.0;
			   if(!item->velocity_.isNull()) // atan2 mustn't be called with x==y==0.0
			   angle = -atan2(item->velocity_.x(),item->velocity_.y()) + PI;

			   // rotate in moving direction
			   item->setTransform(QTransform(cos(angle), sin(angle), -sin(angle), cos(angle), 0, 0));
}

void MovingItem::setupBody()
{
	if(body_)
		world_->DestroyBody(body_);

	// Create body with start-up position and rotation.
	b2BodyDef bodyDef;
	bodyDef.position.Set(x()/SCALE, -y()/SCALE);
	bodyDef.angle = -(angle() * (2 * PI)) / 360.0;
	body_ = world_->CreateBody(&bodyDef);
	// cast to base class, because we don't want to cast polymorph pointers to void*
	QGraphicsItem * basePointer = static_cast<QGraphicsItem*>(this);
	body_->SetUserData(static_cast<void*>(basePointer)); // body saves pointer to this item
}

void MovingItem::applyPhysics(b2World *world, const QPolygonF &poly)
{
	Q_ASSERT(world);
	world_ = world;

	setupBody();

	// Assign shape to polygon body.
	b2PolygonDef shapeDef;
	shapeDef.density = 1.0f;
	shapeDef.friction = 0.5f;
	shapeDef.restitution = 0.3f;
	int n = shapeDef.vertexCount = poly.size();
	for (int i = 0; i < n; ++i) {
		const QPointF &p = poly.at(i);
		shapeDef.vertices[i].Set(p.x()/SCALE, -p.y()/SCALE);
	}
	body_->CreateShape(&shapeDef);

	// Setup default mass.
	body_->SetMassFromShapes();

}

void MovingItem::applyPhysics(b2World *world, float radius)
{
	Q_ASSERT(world);
	world_ = world;

	setupBody();

	// Assign shape to circle body.
	b2CircleDef shapeDef;
	shapeDef.radius = radius/SCALE;
	shapeDef.density = 1.0f;
	shapeDef.friction = 0.5f;
	shapeDef.restitution = 0.3f;
	body_->CreateShape(&shapeDef);

	// Setup default mass.
	body_->SetMassFromShapes();
}

void MovingItem::advance(int phase)
{
	if(phase == 1 && isAdvancing() && body_)
	{
		// Update QGraphicsItem's position and rotation from body.
		b2Vec2 position = body_->GetPosition();
		float32 angle = body_->GetAngle();
		setPos(position.x*SCALE, -position.y*SCALE);
		setAngle(-(angle * 360.0) / (2.0 * PI));

		float velx = body_->GetLinearVelocity().x;
		float vely = -body_->GetLinearVelocity().y;

		// accelerate
		if(!acceleration_.isNull())
		{
			float ax, ay;
			if(!maximumVelocity_.isNull())
			{
				ax = signum(acceleration_.x()) != signum(velx) || qAbs(velx) < maximumVelocity_.x() ? acceleration_.x() : 0.0f;
				ay = signum(acceleration_.y()) != signum(vely) || qAbs(vely) < maximumVelocity_.y() ? acceleration_.y() : 0.0f;
			}
			else
			{
				ax = acceleration_.x();
				ay = acceleration_.y();
			}
			body_->ApplyForce(body_->GetMass()*b2Vec2(ax,-ay),body_->GetWorldCenter());
		}

		// update velocity
		velocity_.setX(velx);
		velocity_.setY(vely);

		// advance animation
		AnimatedItem::advance(phase);
	}
}
