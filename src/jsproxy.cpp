#include "jsproxy.h"
#include <QFile>
#include <QQmlEngine>
#include "vehicle.h"
#include "animateditem.h"
#include "movingitem.h"

// taken from textrix example, retrieve the staticMetaObject
struct QtMetaObject : private QObject
{
public:
    static const QMetaObject *get()
        { return &static_cast<QtMetaObject*>(0)->staticQtMetaObject; }
};

JSProxy::JSProxy(QObject *parent) : QObject(parent)
{
    engine_ = new QJSEngine(parent); // must be QJSEngine, because global object of QQmlEngine cannot be modified

    qmlRegisterType<Vehicle>();
    qmlRegisterType<AnimatedItem>();
    qmlRegisterType<MovingItem>();
    qRegisterMetaType<Vehicle::WormholeState>("Vehicle::WormholeState");

    QJSValue metaObject=	engine_->newQMetaObject(&Vehicle::staticMetaObject);
    engine_->globalObject().setProperty("Vehicle", metaObject);

    // Make qt namespace available in script
    QJSValue Qt = engine_->newQMetaObject(QtMetaObject::get());
    engine_->globalObject().setProperty("Qt", Qt);



 // forwarding of keyPress and keyRelease signals
    QJSValue thisObject = engine_->newQObject(this);
    engine_->globalObject().setProperty("ScriptProxy", thisObject);
}

void JSProxy::evaluateFile(const QString &fileName)
{
    // open script file
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug(qPrintable("evaluateFile: could not open: " +fileName));

    }

    QJSValue result =  engine_->evaluate(file.readAll(), fileName);

    printError(result);
}

void JSProxy::newQObjectWithName(QObject *object, const QString &name, QQmlEngine::ObjectOwnership ownership)
{

    QJSValue value = engine_->newQObject(object);
    QQmlEngine::setObjectOwnership(object,ownership);
    engine_->globalObject().setProperty(name,value);
}

void JSProxy::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event)
    emit signalTimerEvent();
    // TODO use try catch to log exceptions
}

void JSProxy::printError(const QJSValue &result)
{
    if (result.isError())
        qDebug("%s",qPrintable("uncaught exception at line " + QString::number(result.property("lineNumber").toInt()) + " : " + result.toString()));
}

QJSEngine *JSProxy::engine() const
{
    return engine_;
}
