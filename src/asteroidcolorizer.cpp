#include "asteroidcolorizer.h"
#include <QtConcurrentMap>

AsteroidColorizer::AsteroidColorizer()
{
	pixmapsSynchronized_ = false;
	asteroidNames_ << "golevka" << "kleopatra" << "ky26" << "toutatis"; // << "bacchus" ;
	loadDefault();
}

AsteroidColorizer::~AsteroidColorizer()
{
	future_.cancel();
}

void AsteroidColorizer::loadDefault()
{
	if(!defaultImages_.isEmpty()) // then reuse old defaultImages
	{
		asteroidImages_ = defaultImages_;
		imagesIntoPixmaps();
		return;
	}
	foreach(QString name, asteroidNames_)
	{
		ImageList images;
		PixmapList pixmaps;
		for(int i = 0; i< 32; ++i)
		{
                        //load image
                        QImage image(":" + name + "/" + name + QString::number(i) + ".png");
                        image = image.convertToFormat(QImage::Format_Indexed8);
                            image.setColor(image.pixelIndex(0,0),QColor(Qt::transparent).rgba()); // cut out pink background
			int newWidth = image.width() * 2;
			QImage scaledImage = image.scaledToWidth(newWidth,Qt::SmoothTransformation).convertToFormat(QImage::Format_ARGB32);
			images << scaledImage;
                        pixmaps << QPixmap::fromImage(scaledImage);
		}
		asteroidPixmaps_.insert(name,pixmaps);
		pixmapsSynchronized_ = true;
		defaultImages_.insert(name,images);
		asteroidImages_ = defaultImages_;
	}
}

void AsteroidColorizer::startConcurrentTint(const QColor& tintColor)
{
	// tint asteroids in seperate thead
	TintList tintList; // functor
	tintList.color = tintColor;
	asteroidImages_ = defaultImages_;
	future_ = QtConcurrent::map(asteroidImages_,tintList);
	pixmapsSynchronized_ = false;
}

/*static*/ QImage AsteroidColorizer::tinted(const QImage &image, const QColor &color, QPainter::CompositionMode mode)
{
	QImage resultImage(image.size(), QImage::Format_ARGB32_Premultiplied);
	QPainter painter(&resultImage);
	painter.drawImage(0, 0, image);
	painter.setCompositionMode(mode);
	painter.fillRect(resultImage.rect(), color);
	painter.end();
	resultImage.setAlphaChannel(image.alphaChannel());

	return resultImage;
}

void AsteroidColorizer::imagesIntoPixmaps()
{
	if(future_.isRunning() || pixmapsSynchronized_ == true)
		return;

	QHash<QString, PixmapList> tempAsteroidPixmaps;
	foreach(QString name, asteroidNames_)
	{
		const ImageList& images = asteroidImages_.value(name);
		PixmapList pixmaps;
		foreach(QImage image, images)
		{
			pixmaps << QPixmap::fromImage(image);
		}
		tempAsteroidPixmaps.insert(name,pixmaps);
	}
	asteroidPixmaps_.clear();
	asteroidPixmaps_ = tempAsteroidPixmaps;
	pixmapsSynchronized_ = true;
}
