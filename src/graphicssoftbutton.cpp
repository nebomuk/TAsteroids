#include "graphicssoftbutton.h"
#include <QtDebug>

GraphicsSoftButton::GraphicsSoftButton(const QString &svgFilePath) : QGraphicsSvgItem(svgFilePath)
{

}

void GraphicsSoftButton::scaleToWidth(qreal width)
{
    this->setScale(width/this->boundingRect().width());
}

void GraphicsSoftButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    qDebug() << "emit released()";
    emit released();
}

void GraphicsSoftButton::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

    qDebug() << "emit pressed()";

    emit pressed();
}
