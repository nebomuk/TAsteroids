#include <QPainter>
#include <QPen>
#include <QSvgRenderer>
#include "mechanicalcounter.h"

MechanicalCounter::MechanicalCounter(QGraphicsItem * parent): QGraphicsItem(parent)
{
    boundingRect_ = QRectF(0.0,0.0,128.0,64.0);
    digits_ = 4;
    value_ = 0;
    svgRenderer_ = NULL;
    setCacheMode(QGraphicsItem::ItemCoordinateCache);
}

int MechanicalCounter::digits() const
{
    return digits_;
}

int MechanicalCounter::value() const
{
    return value_;
}

const QString MechanicalCounter::digitsFile() const
{
    return digitsFile_;
}

void MechanicalCounter::setValue(int i)
{
    value_ = i;
    update();
}

void MechanicalCounter::setDigits(int i)
{
    digits_ = i;
    update();
}

void MechanicalCounter::setDigitsFile(const QString & i )
{
    digitsFile_ = i;
    if (svgRenderer_)
        delete svgRenderer_;
    svgRenderer_ = new QSvgRenderer;

    bool successful = svgRenderer_->load(digitsFile_);
    if (!successful)
    {
        qDebug("MechanicalCounter::setDigitsFile could not open %s",qPrintable(digitsFile_));
    }
    update();
}


void MechanicalCounter::paint ( QPainter * p, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    const int xOffset = 10;
    const int yOffset = 10;

    p->setRenderHint(QPainter::Antialiasing,true);

    p->save();
    int side = this->height();
    p->scale(side/100.0,side/100.0);

    double width = 100.0 * this->width()/this->height(), height = 100.0;


    QPen pen = p->pen();

    pen.setColor(QColor(160,160,160));
    pen.setWidthF(6.0);
    p->setPen(pen);
    p->setBrush(Qt::black);
    p->drawRoundRect(3,3,width-6,height-6,7,(7*width)/(double)height);


    int w = (width-2*xOffset)/digits_;
    int x = (digits_-1) * w;
    int h = height-2*yOffset;
    int r, c = value_;

    for (int i=0 ; i<digits_ ; i++)
    {
        r = c % 10;
        c = c/10;
        QRect rect(x+xOffset,yOffset,w,h);
        svgRenderer_->render(p,QString("d%1").arg(r),rect);
        x -= w;
    }
    p->restore();
}

