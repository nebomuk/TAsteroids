#ifndef __GLOBALVARIABLES_H__
#define __GLOBALVARIABLES_H__

// all variables here must be const else linker errors will occur

// you can adjust the gamespeed by changing TimeoutInterval
// on windows the game had a "half speed" bug ( prior to Qt 4.6)
const int TimeoutInterval = 40;

const double PI  = 3.141592653589793238512808959406186204433;

const qreal SCALE = 16.0; // scale factor to translate between Box2D and QGraphicsView
static const int B2_ITERATIONS = 10; // Box2D's world iterations
static const float B2_TIMESTEP = 1.0f / 30.0f; // used by Box2D's world

// these types are used by the QGraphicsItem subclasses to make dynamic casting via qgraphicsitem_cast available
enum itemType
{
	TYPE_EMPTY = 0,
	TYPE_ANIMATEDPIXMAPITEM = 2,
	TYPE_MOVINGITEM = 3,
	TYPE_VEHICLE = 4,
};

// forward declarations, this allows headers that include globalvariables.h to store pointers to this classes

class AnimatedItem;
class MovingItem;
class Vehicle;
class GraphicsEngine;
class ScriptProxy;
class MechanicalCounter;
class SvgCache;
class GameState;
class ContactListener;
class AsteroidColorizer;
class SoundEngine;

typedef QList<QImage> ImageList;
typedef QList<QPixmap> PixmapList;

#endif // __GLOBALVARIABLES_H__
