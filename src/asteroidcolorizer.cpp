#include "asteroidcolorizer.h"
#include <QtConcurrentMap>

AsteroidColorizer::AsteroidColorizer()
{
	asteroidNames_ << "golevka" << "kleopatra" << "ky26" << "toutatis"; // << "bacchus" ;
	loadDefault();
}

AsteroidColorizer::~AsteroidColorizer()
{

}

void AsteroidColorizer::loadDefault()
{
	if(!defaultImages_.isEmpty()) // then reuse old defaultImages
	{
		asteroidImages_ = defaultImages_;
		return;
	}
	foreach(QString name, asteroidNames_)
	{
		ImageList images;
		for(int i = 0; i< 32; ++i)
		{
                        //load image
                        QImage image(":" + name + "/" + name + QString::number(i) + ".png");
                        image = image.convertToFormat(QImage::Format_Indexed8);
                            image.setColor(image.pixelIndex(0,0),QColor(Qt::transparent).rgba()); // cut out pink background
//			int newWidth = image.width() * 2;
//			QImage scaledImage = image.scaledToWidth(newWidth,Qt::SmoothTransformation).convertToFormat(QImage::Format_ARGB32);
//			images << scaledImage;
            images << image;
		}
		defaultImages_.insert(name,images);
		asteroidImages_ = defaultImages_;
	}
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

