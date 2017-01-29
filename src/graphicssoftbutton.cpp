#include "graphicssoftbutton.h"
#include <QtDebug>
#include <QTouchEvent>

GraphicsSoftButton::GraphicsSoftButton(const QString &svgFilePath) : QGraphicsSvgItem(svgFilePath)
{
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setAcceptTouchEvents(true);
}

void GraphicsSoftButton::scaleToWidth(qreal width)
{
    this->setScale(width/this->boundingRect().width());
}

bool GraphicsSoftButton::sceneEvent(QEvent * event)
    {
        switch (event->type()) {
        case QEvent::TouchBegin:
        {
            emit pressed();
            return true;
            break;
        }
        case QEvent::TouchUpdate:
        {
           // return true;
            break;
        }
        case QEvent::TouchCancel:
        {
            emit released();
            return true;
            break;
        }
        case QEvent::TouchEnd:
        {
            emit released();
            return true;
            break;
        }
//            QTouchEvent *touchEvent = static_cast<QTouchEvent *>(event);
//            QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();
//            if (touchPoints.count() == 2) {
//                //do the zoom
//                }

        default:
        {

        }
        }
        return QGraphicsItem::sceneEvent(event);
    }


QPainterPath GraphicsSoftButton::shape() const
{
    QPainterPath path;
  path.addEllipse(boundingRect());
  return path;
}

