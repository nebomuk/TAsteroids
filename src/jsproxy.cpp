#include "jsproxy.h"
#include <QFile>
#include <QQmlEngine>
#include "vehicle.h"
#include "animateditem.h"
#include "movingitem.h"

// taken from textrix example, retrieve the staticMetaObject
#if QT_VERSION < 0x060000

struct QtMetaObject : private QObject
{
public:
    static const QMetaObject *get()
        { return &static_cast<QtMetaObject*>(0)->staticQtMetaObject; }
};
#endif

JSProxy::JSProxy(QObject *parent) : QObject(parent)
{
    engine_ = new QJSEngine(parent); // must be QJSEngine, because global object of QQmlEngine cannot be modified
    engine_->installExtensions(QJSEngine::AllExtensions);

#if QT_VERSION >= 0x060000

    qmlRegisterType<Vehicle>("",1.0,0,"Vehicle");
    qmlRegisterType<AnimatedItem>("",1.0,0,"AnimatedSvgItem");
    qmlRegisterType<MovingItem>("",1.0,0,"MovingItem");
#else
    qmlRegisterType<Vehicle>();
    qmlRegisterType<AnimatedItem>();
    qmlRegisterType<MovingItem>();
#endif
    qRegisterMetaType<Vehicle::WormholeState>("Vehicle::WormholeState");

    QJSValue metaObject=	engine_->newQMetaObject(&Vehicle::staticMetaObject);
    engine_->globalObject().setProperty("Vehicle", metaObject);

    // Make Qt:: enums  namespace (colors etc.) available in script
#if QT_VERSION >= 0x060000
    QJSValue Qt = engine_->newQMetaObject(&Qt::staticMetaObject);
#else
    QJSValue Qt = engine_->newQMetaObject(QtMetaObject::get());
#endif
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

QString JSProxy::osName()
{
     #if defined(Q_OS_ANDROID)
     return QLatin1String("android");
     #elif defined(Q_OS_BLACKBERRY)
     return QLatin1String("blackberry");
     #elif defined(Q_OS_IOS)
     return QLatin1String("ios");
     #elif defined(Q_OS_MACOS)
     return QLatin1String("macos");
     #elif defined(Q_OS_TVOS)
     return QLatin1String("tvos");
     #elif defined(Q_OS_WATCHOS)
     return QLatin1String("watchos");
     #elif defined(Q_OS_WINCE)
     return QLatin1String("wince");
     #elif defined(Q_OS_WIN)
     return QLatin1String("windows");
     #elif defined(Q_OS_LINUX)
     return QLatin1String("linux");
     #elif defined(Q_OS_UNIX)
     return QLatin1String("unix");
     #else
     return QLatin1String("unknown");
     #endif
}

QJSEngine *JSProxy::engine() const
{
    return engine_;
}
