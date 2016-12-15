#include "androidhelper.h"
#include "backgroundimageprovider.h"
#include "widgetinteraction.h"

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlContext>
#include <QtQml>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QUrl simpleResourceUrl(QStringLiteral("qrc:/SimpleModelSingleton.qml"));
    qmlRegisterSingletonType(simpleResourceUrl, "my.simplemodel.singleton", 1, 0, "SimpleModel");

    QUrl highScoreResourceUrl(QStringLiteral("qrc:/HighScoreModelSingleton.qml"));
    qmlRegisterSingletonType(highScoreResourceUrl, "my.highscoremodel.singleton", 1, 0, "HighScoreModel");

#ifdef Q_OS_ANDROID
   AndroidHelper helper;
   helper.setScreenOrientation(6);
#endif

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("widgetInteraction",new WidgetInteraction());
    engine.addImageProvider(QLatin1String("backgroundimageprovider"), new BackgroundImageProvider);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
