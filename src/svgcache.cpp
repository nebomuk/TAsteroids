#include "svgcache.h"
#include <QPainter>

SvgCache::SvgCache()
{
	scaleFactor_ = 1.0;
}

/*static*/ QImage SvgCache::renderToImage(QSvgRenderer * renderer, qreal scaleFactor /* = 1.0 */)
{
	if(!renderer)
        return QImage();

	if(!renderer->isValid())
	{
		qDebug("renderToPixmap: no valid svg file.");
        return QImage();
	}

    QImage image(renderer->defaultSize()*scaleFactor,QImage::Format_ARGB32_Premultiplied);

    image.fill(Qt::transparent);
    QPainter painter(&image);
	renderer->render(&painter);
    return image;
}


void SvgCache::update()
{
    images_.clear();
	for(int i = 0; i<svgRenderers_.size(); ++i)
	{
		QSvgRenderer * renderer = svgRenderers_[i];

        images_.push_back(renderToImage(renderer,scaleFactor_));
	}
}

QList<QImage> SvgCache::images() const
{
    return images_;
}
