#ifndef AnimatedItem_H
#define AnimatedItem_H

#include <QGraphicsItem>
#include <QObject>
#include <QList>
#include <QSvgRenderer>
#include <QHash>
#include <cstdlib> // for quot and div
#include "globalvariables.h"


// this class provides frame based animations for QPixmap objects
// use addPixmap or addPixmaps to append QPixmap objects
// animations are very cpu intensive, so most items will be single framed.
// For single frame items its recommended to use setFile to specified an svgFile instead of setRenderer

// this class doesn't inherit QGraphicsSvgItem because QGraphicsItem::DeviceCoordinateCache is hardcoded
// into QGraphicsSvgItem (bug fixed in qt > 4.41) but animations only work with QGraphicsItem::NoCache
// unless you call update() every time a frame changes
// QGraphicsItem::DeviceCoordinateCache is the default cache mode. If you enable pixmapCaching,
// QGraphicsItem::NoCache will be used. QGraphicsItem::NoCache is also recommended if you want to transform the items

// its possible that a warning appears "Couldn't resolve property: ".
// This is due to QSvgRenderer and a specific line in your svg File

class AnimatedItem : public QObject, public QGraphicsItem
{
	Q_OBJECT

	Q_INTERFACES(QGraphicsItem)

	Q_PROPERTY(qreal x READ x WRITE setX STORED false)
	Q_PROPERTY(qreal y READ y WRITE setY STORED false)
	Q_PROPERTY(qreal width READ width STORED false)
	Q_PROPERTY(qreal height READ height STORED false)
	Q_PROPERTY(QString file READ file WRITE setFile STORED false)
	Q_PROPERTY(int frame READ frame WRITE setFrame STORED false)
	Q_PROPERTY(qreal zValue READ zValue WRITE setZValue STORED false)
	Q_PROPERTY(bool visible READ isVisible WRITE setVisible)

	// this only works properly if the parent is an AnimatedItem* too
	Q_PROPERTY(AnimatedItem* parentItem READ parentAnimatedItem WRITE setParentItem)

public:
	AnimatedItem ( QGraphicsItem * parent = 0 );

	void setX(qreal x){setPos(x,y());}
	void setY(qreal y){setPos(x(),y);}


	// wrapper for parentItem() that returns a AnimatedItem*, needed for property
	AnimatedItem* parentAnimatedItem() { return qgraphicsitem_cast<AnimatedItem*>(this->parentItem());}

	// unlike QGraphicsSvgItem, this function doesn't call update()
	void setRenderer(QSvgRenderer* renderer);
	QSvgRenderer* renderer() const { return renderer_; }

	qreal width() const { return boundingRect_.width();}
	qreal height() const { return boundingRect_.height();}

    void addImage(const QImage & image);
    void addImages(const ImageList &images);

	// automatically creates 1 renderer for each filename that is shared among all AnimatedItem objects
	// this is only for single frame objects
	// the internal cache does not use reference counting, all svgs stay inside as long as the application is running
	// setFile also calls virtual fileChanged()
	void setFile(const QString& svgFile);
	const QString & file() const { return file_;} // only available if file has been set with setFile

	// this virtual function is called by AnimatedItem if a new file is assigned
	// the default implementation does nothing
    virtual void fileChanged() {}

	// scale factor used for rendering
	void setScaleFactor(qreal scale){ scaleFactor_ = scale;}
	qreal scaleFactor() const { return scaleFactor_;}

	void setLoopCount(int loops) { loopCount_ = loopsRemaining_ =  loops; }
	int loopCount() const { return loopCount_; }

	void setFrame(int frame);
	inline int frame() const { return currentFrame_; }
    inline int frameCount() const { return images_.size() *frameRateDivisor_;}

    QImage imageAt(int frame) const;

	int loopsRemaining() const { return loopsRemaining_;}

	void setAdvancing(bool advancing){advancing_ = advancing;}
	bool isAdvancing() const {return advancing_;}

	// show the next frame every frameRateDivisor_ frames
	void setFrameRateDivisor(int value) { frameRateDivisor_ = value; }
	int frameRateDivisor() const { return frameRateDivisor_; }

	// drawing offset. the boundingRect() is translated by this offset too
	void setOffset ( const QPointF & offset );
	void setOffset ( qreal x, qreal y ) { offset_ = QPointF(x,y);}

	QPointF offset() const { return offset_;}

	Q_INVOKABLE void setCenterOffset() { setOffset(center(this));}

	// returns the offset to center the item, see the documentation of GraphicsEngine for more details
	static QPointF center(QGraphicsItem * s)
	{return QPointF(-s->boundingRect().width()/2, -s->boundingRect().height()/2);}

    enum { Type = UserType+TYPE_ANIMATEDITEM};
	int type() const {return Type;} // Enable the use of qgraphicsitem_cast with this item.

	QRectF boundingRect() const;
	QPainterPath shape() const; // returns bounding Rect

protected:
	void advance(int phase);

	void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

private:
	void init();
	int frameRateDivisor_;
	int currentFrame_;

	QRectF boundingRect_;

    QImage currentImage_;

    ImageList images_;

	// reference to a hash that stores svgrenderers shared among all objects of AnimatedItem
	static QHash<QString,QSvgRenderer*>& getRendererHash();
	QString file_;

	bool advancing_;
	int loopCount_;
	int loopsRemaining_;
	qreal scaleFactor_;
	QPointF offset_;

	QSvgRenderer * renderer_;
};
#endif
