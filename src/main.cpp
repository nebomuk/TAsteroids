#include "graphicsview.h"
#include "menugameinteraction.h"

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

    // crashes when trying to use ANGLE and opengl driver might not be available
   #ifdef Q_OS_WIN
    QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
    #endif

    app.setQuitOnLastWindowClosed(false);

    app.setApplicationName("TAsteroids");
    app.setOrganizationName("com.blogspot.tasteroids");
	qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));  // seed the random number generator
	app.setWindowIcon(QIcon(":icon.png"));
	app.setOverrideCursor(QCursor(QPixmap(":cursor.png"),2,2));
	QFontDatabase::addApplicationFont(":OCRA.ttf");

    // background image and tap gesture only work in landscape
#ifdef Q_OS_ANDROID
    AndroidHelper helper;
    const int SCREEN_ORIENTATION_SENSOR_LANDSCAPE = 6;
    helper.setScreenOrientation(SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
#endif

     MenuGameInteraction menuGameInteraction;
     menuGameInteraction.showMainMenu();



    //app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
	

	return app.exec();
}
