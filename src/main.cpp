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
#if QT_VERSION > 0x060000
    //qputenv("QSG_RHI_BACKEND", "opengl");
#endif

    // crashes when trying to use ANGLE and opengl driver might not be available
   #ifdef Q_OS_WIN
#if QT_VERSION < 0x060000
  QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
#endif
    #endif

	QApplication app( argc, argv );



    app.setQuitOnLastWindowClosed(false);

    app.setApplicationName("TAsteroids");
    app.setOrganizationName("com.blogspot.tasteroids");
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
