#include "animateditem.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include "svgcache.h"
#include "asteroidcolorizer.h"

AnimatedItem::AnimatedItem(QGraphicsItem * parent)
	: QObject(), QGraphicsItem(parent)
{
	init();
}

void AnimatedItem::init()
{
	advancing_ = true;
	loopCount_ = 0; // loop forever
	loopsRemaining_ = loopCount_;
	scaleFactor_ = 1.0;

	frameRateDivisor_ = 1;
	currentFrame_ = 0;

    renderer_ = Q_NULLPTR;

	this->setCacheMode(QGraphicsItem::NoCache);
}


/*static*/ QHash<QString,QSvgRenderer*>& AnimatedItem::getRendererHash()
{
		static QHash<QString,QSvgRenderer*> rendererHash;
		return rendererHash;
}

QColor AnimatedItem::getTintColor() const
{
    return tintColor_;
}

void AnimatedItem::setTintColor(const QColor &tintColor)
{
    tintColor_ = tintColor;
}

void AnimatedItem::setRenderer(QSvgRenderer* renderer)
{
    renderer_ = renderer;
    images_ << SvgCache::renderToImage(renderer_,scaleFactor_);
}

QImage AnimatedItem::imageAt(int frame) const
{
    return images_.empty() ? QImage() : images_.at(div(frame,frameRateDivisor_).quot);
}

void AnimatedItem::setFile(const QString& svgFile)
{
	file_ = svgFile;

	QHash<QString,QSvgRenderer*> & rendererHashRef = getRendererHash();
	if(rendererHashRef.contains(svgFile))
	{
		setRenderer(rendererHashRef.value(svgFile));
	}
	else
	{
		setRenderer(new QSvgRenderer(svgFile));
		rendererHashRef.insert(svgFile,renderer());
	}

    images_.clear();
    images_ << SvgCache::renderToImage(renderer_,scaleFactor_);
	boundingRect_ = QRectF(offset_,renderer()->defaultSize());
	fileChanged(); // virtual, does nothing by default
}

void AnimatedItem::addImage(const QImage &image)
{
    images_+=image;
	//currentPixmap_ = pixmapAt(currentFrame_);
    boundingRect_ = QRectF(offset_,imageAt(currentFrame_).size());

}
void AnimatedItem::addImages(const ImageList &images)
{
    images_+=images;
	//currentPixmap_ = pixmapAt(currentFrame_);
    boundingRect_ = QRectF(offset_,imageAt(currentFrame_).size());
}

void AnimatedItem::setOffset(const QPointF &offset)
{
	offset_ = offset;
	boundingRect_.translate(offset_);
}

QRectF AnimatedItem::boundingRect() const
{
	qreal pw = 1.0; // pen width
	return boundingRect_.adjusted(-pw/2, -pw/2, pw/2, pw/2);
}
QPainterPath AnimatedItem::shape() const
{
	QPainterPath path;
	path.addRect(boundingRect_);
	return path;
}

void AnimatedItem::advance(int phase)
{
	// quotient is internal frameCount
	if(advancing_ && phase == 1 && frameCount()/frameRateDivisor() > 1)
	{
		if(frame() >= frameCount()-1)
		{
			setFrame(0);
			if(loopsRemaining_ > 0)
				--loopsRemaining_;
		}
		if(loopsRemaining_ > 0 || loopCount_ == 0)
		{
			setFrame(frame() + 1);
		}
	}
}

void AnimatedItem::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);

    if(images_.size() > 0)
    {
        QImage img = imageAt(currentFrame_);
        if(tintColor_.isValid())
        {
            img = AsteroidColorizer::tinted(img,QColor(Qt::red),QPainter::CompositionMode_Overlay);
        }
        painter->drawImage(offset_,img);
    }
	else if(renderer())
		renderer()->render(painter,QRectF(offset_,this->renderer()->defaultSize()));
}

void AnimatedItem::setFrame(int value)
{
    int newFrame = value % (images_.size() * frameRateDivisor_); // operator  % has same precedence as *
	if (currentFrame_ != newFrame)
	{
		currentFrame_ = newFrame;
		//currentFrameInternal_ = div(currentFrame_,frameRateDivisor_).quot;
			//currentPixmap_ = pixmapAt(frame());
			prepareGeometryChange();
	}
}

