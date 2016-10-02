#ifndef SVGCACHE_H
#define SVGCACHE_H

#include <QList>
#include <QSvgRenderer>

class QSvgRenderer;

// This class caches the svgRenderers contents into pixmaps
class SvgCache
{
public:
	SvgCache();
	void setScaleFactor(qreal scale){ scaleFactor_ = scale;}
	qreal scaleFactor() const { return scaleFactor_;}
	void addSvgRenderer(QSvgRenderer * renderer){svgRenderers_+=renderer; update();}
	void addSvgRenderers(const QList<QSvgRenderer*> & renderers){svgRenderers_+=renderers;update();}


    static QImage renderToImage(QSvgRenderer * renderer, qreal scaleFactor = 1.0);

	const QList<QSvgRenderer*> & svgRenderers() const { return svgRenderers_;}
    int size() const { return svgRenderers_.empty() ? images_.size() : svgRenderers_.size();}
    void clear(){svgRenderers_.clear(); images_.clear();}

	void update();

    QList<QImage> images() const;

private:
    QList<QSvgRenderer*>  svgRenderers_;
    QList<QImage> images_;
	qreal scaleFactor_;

};

#endif // SvgCache_H
