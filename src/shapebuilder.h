#ifndef SHAPEBUILDER_H
#define SHAPEBUILDER_H

#include <QGraphicsPolygonItem>
#include "Box2D/Box2D.h"



class ShapeBuilder
{
public:
    ShapeBuilder();

private:
    // create some colored fun polygons that you can push around
    void addPolygons(QGraphicsScene *scene, b2World *world);
    QGraphicsRectItem * addRect(QGraphicsScene *scene, b2World * world, QPointF pos, QSizeF size);
};

#endif // SHAPEBUILDER_H
