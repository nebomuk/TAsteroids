#ifndef JSPROXY_H
#define JSPROXY_H

#include <QObject>
#include <QQmlEngine>


/// This class serves as a proxy between the script environments and GraphicsView
/// replacement for ScriptProxy
class JSProxy : public QObject
{
    Q_OBJECT
public:
    explicit JSProxy(QObject *parent = 0);

    void evaluateFile(const QString &fileName);

    void newQObjectWithName(QObject * Object, const QString& name, QQmlEngine::ObjectOwnership ownership = QQmlEngine::CppOwnership);

    // emits signalTimerEvent
    void timerEvent ( QTimerEvent * event );




    QJSEngine *engine() const;

signals:
    void signalKeyPress(int);
    void signalKeyRelease(int);
    void signalGestureStarted(int);
    void signalGestureFinished(int);
    void signalTimerEvent();


signals:

public slots:

private:
    static void printError(const QJSValue & result);

    QJSEngine * engine_;

};

#endif // JSPROXY_H
