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
    ImageList imagesForAsteroid(const QString asteroidName, QColor color);

	// loads resource images and pixmaps into asteroidImages_ and asteroidPixmaps_

private:
    void loadDefault();
    ImageList coloredImageList(QString key);

	QStringList asteroidNames_;
	QHash< QString, ImageList> defaultImages_;
	QHash< QString, ImageList> asteroidImages_;
    QColor cacheColor_;  // the color of the cached asteroid images
};






#endif // ASTEROIDCOLORIZER_H


