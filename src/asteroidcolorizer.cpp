#include "asteroidcolorizer.h"

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

    for(QString name : asteroidNames_)
	{
		ImageList images;
		for(int i = 0; i< 32; ++i)
		{
                        //load image
                        QImage image(":images/asteroid/" + name + "/" + name + QString::number(i) + ".png");
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
//	resultImage.setAlphaChannel(image.alphaChannel());

	return resultImage;
}


ImageList AsteroidColorizer::imagesForAsteroid(const QString asteroidName, QColor color)
{
    Q_ASSERT(defaultImages_.contains(asteroidName));

    if(!color.isValid())
    {
         return defaultImages_.value(asteroidName);
    }
    else
    {
        if(cacheColor_ != color)
        {

            // clear cache, cause we cache only 1 color at a time
            asteroidImages_.clear();
            cacheColor_ = color;

            ImageList colored = coloredImageList(asteroidName);
            asteroidImages_.insert(asteroidName,colored);
            return colored;
        }
        else
        {
            if(asteroidImages_.contains(asteroidName))
            {
                return asteroidImages_[asteroidName];
            }
            else
            {
                ImageList colored = coloredImageList(asteroidName);
                asteroidImages_.insert(asteroidName,colored);
                return colored;
            }

        }

    }
}

ImageList AsteroidColorizer::coloredImageList(QString key)
{
     const ImageList uncolored = defaultImages_[key];
    ImageList colored;
    for(QImage image : uncolored)
    {
        colored.append(tinted(image,cacheColor_));
    }
    return colored;
}


