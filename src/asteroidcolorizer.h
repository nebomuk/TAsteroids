#ifndef ASTEROIDCOLORIZER_H
#define ASTEROIDCOLORIZER_H

#include <QImage>
#include <QPainter>
#include <QHash>
#include <QBitmap>
#include <QFuture>
#include "globalvariables.h"

/*
  AsteroidColorizer loads the asteroid images from the resources and caches them into pixmaps and images.
  It can colorize (tint) the images concurrently (asynchronously) via startConcurrentTint and load the tintedImages into the pixmaps via
   imageIntoPixmaps.

   asteroidImages_ acts as a buffer to store the tinted images that get converted into asteroidPixmaps_
   defaultImages_ are the images that are loaded from the resources
   note that image tinting can only be used on QImages because QPixmaps can not be used outside the Gui-Thread
 */

class AsteroidColorizer
{
public:

	// loads all asteroid images from resource files
	AsteroidColorizer();
	~AsteroidColorizer();

	void startConcurrentTint(const QColor& tintColor);

	// copys all asteroidImages_ to the asteroidPixmaps_;
	// convert the cached images to pixmaps this must happen in the gui thread
	void imagesIntoPixmaps();

	// taken from ImageTint in graphics dojo
	// Tint an image with the specified color and return it as a new image
	// format must be QImage::Format_ARGB32
	static QImage tinted(const QImage &image, const QColor &color, QPainter::CompositionMode mode = QPainter::CompositionMode_Overlay);

	// get the pixmaps for a particular asteroid
	inline PixmapList pixmapsForAsteroid(const QString asteroidName);

	// loads resource images and pixmaps into asteroidImages_ and asteroidPixmaps_
	void loadDefault();

private:
	// this functor calls tinted on a QList<QImage> but with argument color binded
	struct TintList
	{
		TintList() : color(0,0,0) {}
		inline void operator()(ImageList& images);
		QColor color;
	};

	QStringList asteroidNames_;
	QHash< QString, ImageList> defaultImages_;
	QHash< QString, ImageList> asteroidImages_;
	QHash< QString, PixmapList> asteroidPixmaps_;
	QFuture<void > future_; // a future that won't contain results
	bool pixmapsSynchronized_; // true if asteroidImages_ have been converted to asteroidPixmaps_
};

void AsteroidColorizer::TintList::operator ()(ImageList& images)
{
   for(int i = 0; i< images.size(); ++i)
   {
	   images[i] = tinted(images[i],color);
   }
}

PixmapList AsteroidColorizer::pixmapsForAsteroid(const QString asteroidName)
{
	return asteroidPixmaps_.value(asteroidName);
}


#endif // ASTEROIDCOLORIZER_H


