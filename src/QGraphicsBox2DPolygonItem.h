#ifndef QGRAPHICSBOX2DPOLYGONITEM_H
#define QGRAPHICSBOX2DPOLYGONITEM_H

#include "Box2D/Box2D.h"
#include "globalvariables.h"
#include <QGraphicsPolygonItem>

// WARNING item is not scalable for qt < 4.6

class QGraphicsBox2DPolygonItem : public QGraphicsPolygonItem
{
public:
	QGraphicsBox2DPolygonItem(b2World *world, QGraphicsItem *parent = 0)
		: QGraphicsPolygonItem(parent), _world(world), _body(0), angle_(0.0)
	{ }

	~QGraphicsBox2DPolygonItem()
	{
		_world->DestroyBody(_body);
	}
#if QT_VERSION < 0x040600
	void setRotation(qreal angle)
	{
		angle_ = angle;
		qreal radAngle = angle_*PI/180;
		setTransform(QTransform(cos(radAngle), sin(radAngle), -sin(radAngle), cos(radAngle), 0, 0));
	}
	qreal rotation() const { return angle_; }
#endif

	void setup()
	{
		// Create body with start-up position and _rotation.
		b2BodyDef bodyDef;
		bodyDef.position.Set(x()/SCALE, -y()/SCALE);
		bodyDef.angle = -(rotation() * (2 * PI)) / 360.0;
		_body = _world->CreateBody(&bodyDef);

		// Assign shape to polygon body.
		b2PolygonDef shapeDef;
		shapeDef.density = 1.0f;
		shapeDef.friction = 0.5f;
		shapeDef.restitution = 0.3f;
		const QPolygonF &poly = polygon();
		int n = shapeDef.vertexCount = poly.size();
		for (int i = 0; i < n; ++i) {
			const QPointF &p = poly.at(i);
			shapeDef.vertices[i].Set(p.x()/SCALE, -p.y()/SCALE);
		}
		_body->CreateShape(&shapeDef);

		// Setup default mass.
		_body->SetMassFromShapes();
		// cast to base class, because we don't want to cast polymorph pointers to void*
		QGraphicsItem * basePointer = static_cast<QGraphicsItem*>(this);
		_body->SetUserData(static_cast<void*>(basePointer));
	}

	void adjust()
	{
		// Update QGraphicsItem's position and _rotation from body.
		b2Vec2 position = _body->GetPosition();
		float32 angle = _body->GetAngle();
		setPos(position.x*SCALE, -position.y*SCALE);
		setRotation(-(angle * 360.0) / (2 * PI));
	}

private:
	b2World *_world;
	b2Body *_body;
	qreal angle_;
};

#endif // QGRAPHICSBOX2DPOLYGONITEM_H
