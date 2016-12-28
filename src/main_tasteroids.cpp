#include <QApplication>
#include <QTime>
#include <QInputDialog>
#include "androidhelper.h"
#include "graphicsview.h"


// main entry for the game only
int main(int argc, char ** argv)
{
	QApplication app( argc, argv );
	app.setApplicationName("tasteroids");
	app.setOrganizationName("Taiko");
#if QT_VERSION > 0x040600 && QT_VERSION < 0x050000
	app.setGraphicsSystem("raster");
#endif
	qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));  // seed the random number generator
	//int playerCount = QInputDialog::getInt(NULL,QObject::tr("Player Count"),QObject::tr("Enter number of players"),
	//									   1,1,2);


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
