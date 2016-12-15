#ifndef BACKGROUNDIMAGEPROVIDER_H
#define BACKGROUNDIMAGEPROVIDER_H

#include <QQuickImageProvider>



class BackgroundImageProvider : public QQuickImageProvider
{
public:
    BackgroundImageProvider();


    // QQuickImageProvider interface
public:
    virtual QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) Q_DECL_OVERRIDE;
};

#endif // BACKGROUNDIMAGEPROVIDER_H
