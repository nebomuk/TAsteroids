#include "graphicsview.h"

#include <QApplication>
#include <QTime>
#include <QFontDatabase>
#include <QIcon>

#ifdef Q_OS_ANDROID
#include "androidhelper.h"
#endif



int main(int argc, char ** argv)
{
	QApplication app( argc, argv );

	// init resources that are contained in the static library (the actual game)


	app.setApplicationName("tasteroids");
	app.setOrganizationName("Taiko");
	qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));  // seed the random number generator
	app.setWindowIcon(QIcon(":icon.png"));
	app.setOverrideCursor(QCursor(QPixmap(":cursor.png"),2,2));
	QFontDatabase::addApplicationFont(":OCRA.ttf");

    GraphicsView view;
    // background image and tap gesture only work in landscape
#ifdef Q_OS_ANDROID
    AndroidHelper helper;
    const int SCREEN_ORIENTATION_SENSOR_LANDSCAPE = 6;
    helper.setScreenOrientation(SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
    QObject::connect(&app,&QApplication::applicationStateChanged,&view,&GraphicsView::onApplicationStateChanged);
#endif

    //view.setPlayerCount(playerCount);
    view.restart();
    view.show();

    app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
	

	return app.exec();
}
