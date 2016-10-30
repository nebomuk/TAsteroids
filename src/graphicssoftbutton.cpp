#include "graphicssoftbutton.h"

GraphicsSoftButton::GraphicsSoftButton(const QString &svgFilePath) : QGraphicsSvgItem(svgFilePath)
{

}

void GraphicsSoftButton::scaleToWidth(qreal width)
{
    this->setScale(width/this->boundingRect().width());
}

void GraphicsSoftButton::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit pressed();
}
