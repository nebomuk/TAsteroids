#include "graphicssoftbutton.h"
#include <QtDebug>

GraphicsSoftButton::GraphicsSoftButton(const QString &svgFilePath) : QGraphicsSvgItem(svgFilePath)
{
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);

}

void GraphicsSoftButton::scaleToWidth(qreal width)
{
    this->setScale(width/this->boundingRect().width());
}

void GraphicsSoftButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
//    qDebug() << "emit released()";
//    emit released();
}

void GraphicsSoftButton::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

//    qDebug() << "emit pressed()";

//    emit pressed();
}

QPainterPath GraphicsSoftButton::shape() const
{
    QPainterPath path;
  path.addEllipse(boundingRect());
  return path;
}

