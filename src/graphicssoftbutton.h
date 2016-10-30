#ifndef GRAPHICSSOFTBUTTON_H
#define GRAPHICSSOFTBUTTON_H

#include <QGraphicsSvgItem>
#include <QGraphicsSceneMouseEvent>


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


protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

signals:
    void pressed();
};

#endif // GRAPHICSSOFTBUTTON_H
