#ifndef GRAPHICSSOFTBUTTON_H
#define GRAPHICSSOFTBUTTON_H

#include <QGraphicsSvgItem>
#include <QGraphicsSceneMouseEvent>
#include "globalvariables.h"


///
/// \brief The GraphicsSoftButton class is a graphics item that emits the pressed() signal
/// so it can be used as a button for touchscreens in a QGraphicsScene
///
class GraphicsSoftButton : public QGraphicsSvgItem
{
    Q_OBJECT

public:
    GraphicsSoftButton(const QString& svgFilePath);

    ///
    /// scale to the desired with, keep aspect ratio
    ///
    void scaleToWidth(qreal width);

    enum { Type = UserType+TYPE_GRAPHICS_SOFT_BUTTON};
    int type() const {return Type;} // Enable the use of qgraphicsitem_cast with this item.


protected:
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    virtual QPainterPath shape() const Q_DECL_OVERRIDE;

signals:
    void pressed();
    void released();
};

#endif // GRAPHICSSOFTBUTTON_H
