#include "backgroundimageprovider.h"

#include <QScreen>
#include <QApplication>

BackgroundImageProvider::BackgroundImageProvider()  : QQuickImageProvider(QQuickImageProvider::Image)
{

}


QImage BackgroundImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    QScreen* screen = qApp->primaryScreen();
    int height = screen->size().height();
    QImage image(QStringLiteral(":/menu.png"));

    // TODO last build scaled to height and was looking fine
    QImage scaled;
    if(height > image.height())
    {
        // use scaled to height because aspect ratio of the image is 1.8, much wider than all tablets
        // and fill the vertical space with border image's stretching algorithm
        scaled = image.scaledToHeight(height);
    }
    else
    {
        scaled = image;
    }

    if (size)
    {
                *size = scaled.size();
    }
    return scaled;
}
