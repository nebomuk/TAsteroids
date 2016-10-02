#include "scriptproxy.h"
#include "vehicle.h"

// make Vehicle type available in QtScript
Q_DECLARE_METATYPE(Vehicle*)
Q_DECLARE_METATYPE(QList<Vehicle*>)
Q_DECLARE_METATYPE(MovingItem*)
Q_DECLARE_METATYPE(AnimatedItem*)

// taken from textrix example, retrieve the staticMetaObject
struct QtMetaObject : private QObject
{
public:
	static const QMetaObject *get()
		{ return &static_cast<QtMetaObject*>(0)->staticQtMetaObject; }
};

void ScriptProxy::timerEvent ( QTimerEvent * event )
{
	Q_UNUSED(event)
	emit signalTimerEvent();
	if(engine_->hasUncaughtException())
	{
		printError(engine_->uncaughtException ());
	}
}

static QScriptValue VehicleToScriptValue(QScriptEngine *engine, Vehicle* const &in)
{ return engine->newQObject(in,QScriptEngine::QtOwnership,QScriptEngine::AutoCreateDynamicProperties); }

static void VehicleFromScriptValue(const QScriptValue &object, Vehicle* &out)
{ out = qobject_cast<Vehicle*>(object.toQObject()); }

static QScriptValue MovingItemToScriptValue(QScriptEngine *engine, MovingItem* const &in)
{ return engine->newQObject(in,QScriptEngine::QtOwnership,QScriptEngine::AutoCreateDynamicProperties); }

static void MovingItemFromScriptValue(const QScriptValue &object, MovingItem* &out)
{ out = qobject_cast<MovingItem*>(object.toQObject()); }

static QScriptValue AnimatedItemToScriptValue(QScriptEngine *engine, AnimatedItem* const &in)
{ return engine->newQObject(in,QScriptEngine::QtOwnership,QScriptEngine::AutoCreateDynamicProperties); }

static void AnimatedItemFromScriptValue(const QScriptValue &object, AnimatedItem* &out)
{ out = qobject_cast<AnimatedItem*>(object.toQObject()); }

// checks if a script QObject wrapper's wrapped QObject* is NULL
static QScriptValue isNullQObject(QScriptContext *context, QScriptEngine *engine)
	 {
		//qDebug("%d",!static_cast<bool>(context->argument(0).toQObject()));
		 return QScriptValue(engine,!static_cast<bool>(context->argument(0).toQObject()));
	 }

// ctor
ScriptProxy::ScriptProxy(QObject *parent) :
	QObject(parent)
{
	engine_ = new QScriptEngine(this);
	// register Vehicle type
	qScriptRegisterMetaType(engine_, VehicleToScriptValue, VehicleFromScriptValue);
	qScriptRegisterSequenceMetaType< QList<Vehicle*> >(engine_);

	qScriptRegisterMetaType(engine_, MovingItemToScriptValue, MovingItemFromScriptValue);
	qScriptRegisterMetaType(engine_, AnimatedItemToScriptValue, AnimatedItemFromScriptValue);

	engine_->globalObject().setProperty("isNullQObject",engine_->newFunction(isNullQObject));

	QScriptValue metaObject=	engine_->newQMetaObject(&Vehicle::staticMetaObject);
	engine_->globalObject().setProperty("Vehicle", metaObject);

	// Make qt namespace available in script
	QScriptValue Qt = engine_->newQMetaObject(QtMetaObject::get());
	engine_->globalObject().setProperty("Qt", Qt);



 // forwarding of keyPress and keyRelease signals
	QScriptValue thisObject = engine_->newQObject(this);
	engine_->globalObject().setProperty("ScriptProxy", thisObject);
}

/*static*/ QScriptValue ScriptProxy::evaluateFile(QScriptEngine * engine, const QString &fileName)
{
	// open script file
	QFile file(fileName);
	if(!file.open(QIODevice::ReadOnly))
	{
		qDebug(qPrintable("evaluateFile: could not open: " +fileName));
		return QScriptValue();
	}

	QScriptValue result =  engine->evaluate(file.readAll(), fileName);

	printError(result);

	return result;
}

void ScriptProxy::newQObjectByName(QObject * Object,const QString& name, QScriptEngine::ValueOwnership ownership, const QScriptEngine::QObjectWrapOptions & options)
{
	if(!Object)
	{
		qDebug("newQObjectByName: object is NULL");
		return;
	}
	QScriptValue value = engine_->newQObject(Object,ownership,options);
	engine_->globalObject().setProperty(name, value);
	printError(value);
}

/*static*/ void ScriptProxy::printError(const QScriptValue & value)
{
if (value.engine() && value.engine()->hasUncaughtException())
	{
		int line = value.engine()->uncaughtExceptionLineNumber();
		qDebug("%s",qPrintable("uncaught exception at line " + QString::number(line) + " : " + value.toString()));

		foreach(QString bt, value.engine()->uncaughtExceptionBacktrace())
			qDebug("%s",qPrintable(bt));
	}
}
