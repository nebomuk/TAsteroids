#include <QApplication>
#include <QTime>
#include <QInputDialog>
#include "graphicsview.h"

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
	//view.setPlayerCount(playerCount);
	view.restart();
	view.show();

	app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
	return app.exec();
}
