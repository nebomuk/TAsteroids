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


	// taken from ImageTint in graphics dojo
	// Tint an image with the specified color and return it as a new image
	// format must be QImage::Format_ARGB32
	static QImage tinted(const QImage &image, const QColor &color, QPainter::CompositionMode mode = QPainter::CompositionMode_Overlay);

	// get the pixmaps for a particular asteroid
    inline ImageList imagesForAsteroid(const QString asteroidName);

	// loads resource images and pixmaps into asteroidImages_ and asteroidPixmaps_
	void loadDefault();

private:
	QStringList asteroidNames_;
	QHash< QString, ImageList> defaultImages_;
	QHash< QString, ImageList> asteroidImages_;
};



ImageList AsteroidColorizer::imagesForAsteroid(const QString asteroidName)
{
    return asteroidImages_.value(asteroidName);
}


#endif // ASTEROIDCOLORIZER_H


