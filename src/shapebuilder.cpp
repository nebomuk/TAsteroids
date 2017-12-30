#include "QGraphicsBox2DPolygonItem.h"
#include "shapebuilder.h"
#include "globalvariables.h"

#include <QGraphicsScene>

ShapeBuilder::ShapeBuilder()
{


}

// for testing purposes only, adds a physics aware rectangle
QGraphicsRectItem * ShapeBuilder::addRect(QGraphicsScene *scene, b2World * world, QPointF pos, QSizeF size)
{
    b2BodyDef bodyDef;
    b2PolygonDef shapeDef;
    QGraphicsRectItem *groundItem = scene->addRect(-size.width()/2, -size.height()/2,size.width(),size.height());
    groundItem->setBrush(QColor(178, 192, 160));
    groundItem->setPos(pos);
    groundItem->setPen(Qt::NoPen);
    bodyDef.position.Set(pos.x()/SCALE, -pos.y()/SCALE);
    shapeDef.SetAsBox(size.width()/2/SCALE, size.height()/2/SCALE);
    b2Body * groundBody = world->CreateBody(&bodyDef);
    groundBody->SetUserData(static_cast<void*>(groundItem));
    groundBody->CreateShape(&shapeDef);
    return groundItem;
}

void ShapeBuilder::addPolygons(QGraphicsScene *scene, b2World * world)
{
    QPolygonF poly;
    poly << QPointF(0, -10)*5 << QPointF(-5, 0)*5 << QPointF(5, 0)*5;
    for (int i = 0; i < 3; ++i) {
        QGraphicsBox2DPolygonItem *polygon = new QGraphicsBox2DPolygonItem(world);
        polygon->setPos(600, -400);
        polygon->setRotation(qrand() % 360);
        polygon->setPolygon(poly);
        polygon->setBrush(QColor(128 + qrand() % 128, 128 + qrand() % 128, 128 + qrand() % 128));
        polygon->setup();
        scene->addItem(polygon);
    }
}
