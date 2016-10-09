#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

#include <QObject>
#include <QScriptEngine>
#include <QFile>
#include "globalvariables.h"

// This class serves as a proxy between the script environments and GraphicsView
class ScriptProxy : public QObject
{
Q_OBJECT
public:
	ScriptProxy(QObject *parent = 0);
	static QScriptValue evaluateFile(QScriptEngine * engine, const QString &fileName);
	QScriptValue evaluateFile(const QString &fileName){return evaluateFile(engine_,fileName);}

	void newQObjectByName(QObject * Object, const QString& name, QScriptEngine::ValueOwnership ownership = QScriptEngine::QtOwnership,
						  const QScriptEngine::QObjectWrapOptions & options =0);

	// emits signalTimerEvent
	void timerEvent ( QTimerEvent * event );

	// error handling, print errors if there are some
	static void printError(const QScriptValue & value);
	QScriptEngine * engine() const { return engine_;}


signals:
	void signalKeyPress(int);
	void signalKeyRelease(int);
    void signalGestureStarted(int);
    void signalGestureFinished(int);
	void signalTimerEvent();

private:
	QScriptEngine * engine_;

};

#endif // SCRIPTMANAGER_H
