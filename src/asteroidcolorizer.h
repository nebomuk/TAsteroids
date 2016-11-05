#ifndef ASTEROIDCOLORIZER_H
#define ASTEROIDCOLORIZER_H

#include <QImage>
#include <QPainter>
#include <QHash>
#include <QBitmap>
#include <QFuture>
#include "globalvariables.h"

///
/// \brief The AsteroidColorizer class loads asteroid images and colorizes them
///

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


