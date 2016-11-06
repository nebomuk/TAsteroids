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


QPainterPath GraphicsSoftButton::shape() const
{
    QPainterPath path;
  path.addEllipse(boundingRect());
  return path;
}

