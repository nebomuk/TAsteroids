#include <QApplication>
#include <QFontDatabase>
#include <QIcon>
#include <QTime>

#include "shapedmenu.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
		
	// init resources that are contained in the static library (the actual game)
//	Q_INIT_RESOURCE(bacchus);
//	Q_INIT_RESOURCE(digits_images);
//	Q_INIT_RESOURCE(explosion_images);
//	Q_INIT_RESOURCE(golevka);
//	Q_INIT_RESOURCE(hitpointsBar_images);
//	Q_INIT_RESOURCE(images);
//	Q_INIT_RESOURCE(ky26);
//	Q_INIT_RESOURCE(kleopatra);
//	Q_INIT_RESOURCE(script);
//	Q_INIT_RESOURCE(toutatis);
//	Q_INIT_RESOURCE(sounds);
//	Q_INIT_RESOURCE(menu_files);
	
	qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));  // seed the random number generator
	app.setApplicationName("tasteroids");
	app.setOrganizationName("Taiko");
	app.setWindowIcon(QIcon(":icon.png"));
	app.setOverrideCursor(QCursor(QPixmap(":cursor.png"),2,2));
    //QApplication::setStyle(new QPlastiqueStyle);
	QFontDatabase::addApplicationFont(":OCRA.ttf");

	ShapedMenu menu;
	menu.show();
	return app.exec();
}
