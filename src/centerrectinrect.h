#ifndef __CENTERRECTINRECT_H__
#define __CENTERRECTINRECT_H__

// returns the position in outerRect coordinates
// where the innerRect must be to be in the middle of the outerRect

#include <QRectF>

inline QPoint centerRectInRect(QRect outerRect, QRect innerRect)
{
	return QPoint( (outerRect.width()-innerRect.width())/2 , (outerRect.height()-innerRect.height())/2);
}

inline QPointF centerRectInRect(QRectF outerRect, QRectF innerRect)
{
	return QPointF( (outerRect.width()-innerRect.width())/2 , (outerRect.height()-innerRect.height())/2);
}

#endif // __CENTERRECTINRECT_H__
