#include "svgcache.h"
#include <QPainter>

SvgCache::SvgCache()
{
	scaleFactor_ = 1.0;
}

/*static*/ QPixmap SvgCache::renderToPixmap(QSvgRenderer * renderer, qreal scaleFactor /* = 1.0 */)
{
	if(!renderer)
		return QPixmap();

	if(!renderer->isValid())
	{
		qDebug("renderToPixmap: no valid svg file.");
		return QPixmap();
	}

	QPixmap pixmap(renderer->defaultSize()*scaleFactor);

	pixmap.fill(Qt::transparent);
	QPainter painter(&pixmap);
	renderer->render(&painter);
	return pixmap;
}


void SvgCache::update()
{
	pixmaps_.clear();
	for(int i = 0; i<svgRenderers_.size(); ++i)
	{
		QSvgRenderer * renderer = svgRenderers_[i];

		pixmaps_.push_back(renderToPixmap(renderer,scaleFactor_));
	}
}
