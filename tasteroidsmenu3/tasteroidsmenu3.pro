TARGET = tasteroidsmenu
TEMPLATE = lib
#TEMPLATE = app
CONFIG += staticlib
CONFIG += warn_on
CONFIG += create_prl
CONFIG += link_prl
CONFIG -= debug_and_release debug_and_release_target
DESTDIR = .
MOC_DIR = build
RCC_DIR = build
UI_DIR = build
OBJECTS_DIR = build
INCLUDEPATH += ../tasteroids6/src/ ../Box2D src
QMAKE_LIBDIR += ../Box2D ../tasteroids6
LIBS += -lBox2D -ltasteroids
QT += opengl \
	svg \
	script
RESOURCES = ../menu_files.qrc
HEADERS += src/stackedwidget.h \
	src/shapedmenu.h \
	src/point3d.h \
	src/model.h \
	src/view.h \
	src/openglscene.h \
	src/highscoreview.h
SOURCES += src/stackedwidget.cpp \
	src/shapedmenu.cpp \
	src/model.cpp \
	src/openglscene.cpp \
	src/main.cpp \
	src/highscoreview.cpp
FORMS += ui/stackedwidget.ui
