#ifndef MECHANICALCOUNTER_H
#define MECHANICALCOUNTER_H


#include <QGraphicsItem>
#include <QSvgRenderer>
/**
  Counter shows counter with digits taken from input svg file.
  Feel free to add your own themes. To do that please follow id names int svg file.
*/

class MechanicalCounter : public QGraphicsItem
{
public:
        MechanicalCounter (QGraphicsItem * parent = NULL);

	int digits() const;
	int value() const;
	const QString digitsFile() const;

	void setValue(int);
	void setDigits(int);

        void resize(const QSizeF& size){boundingRect_ = QRectF(QPointF(0.0,0.0),size);}
	void resize ( qreal w, qreal h ){resize(QSizeF(w,h));}

        QRectF boundingRect()  const { return boundingRect_;}

        qreal height() const { return boundingRect_.height();}
        qreal width() const { return boundingRect_.width();}

	/** Sets filepath to svg file with digits */
	void setDigitsFile(const QString & );

protected:
	void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

        int digits_;
        int value_;
        QString digitsFile_;
        QSvgRenderer * svgRenderer_;
        QRectF boundingRect_;
};

#endif // MECHANICALCOUNTER_H
