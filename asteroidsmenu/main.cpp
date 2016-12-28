#include "androidhelper.h"
#include "widgetinteraction.h"

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlContext>
#include <QtQml>
#include <QScreen>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QUrl highScoreResourceUrl(QStringLiteral("qrc:/HighScoreModelSingleton.qml"));
    qmlRegisterSingletonType(highScoreResourceUrl, "my.highscoremodel.singleton", 1, 0, "HighScoreModel");

#ifdef Q_OS_ANDROID
   AndroidHelper helper;
   helper.setScreenOrientation(6);
#endif

    QQmlApplicationEngine engine;
    engine.rootContext()
            ->setContextProperty("screenPixelDensity", QGuiApplication::primaryScreen()->physicalDotsPerInch() * QGuiApplication::primaryScreen()->devicePixelRatio());
    engine.rootContext()->setContextProperty("widgetInteraction",new WidgetInteraction());
    engine.load(QUrl(QStringLiteral("qrc:/mainTouch.qml")));



    return app.exec();
}
