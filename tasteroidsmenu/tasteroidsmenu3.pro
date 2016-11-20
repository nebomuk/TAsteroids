TARGET = tasteroidsmenu
#TEMPLATE = lib
TEMPLATE = app
#CONFIG += staticlib
CONFIG += warn_on
CONFIG += create_prl
CONFIG += link_prl
CONFIG -= debug_and_release debug_and_release_target
DESTDIR = .
MOC_DIR = build
RCC_DIR = build
UI_DIR = build
OBJECTS_DIR = build
INCLUDEPATH += ../src/
#QMAKE_LIBDIR += ../Box2D ../tasteroids6
#LIBS += -lBox2D -ltasteroids
QT += opengl \
        concurrent \
        widgets

RESOURCES = ../menu_files.qrc
HEADERS += src/stackedwidget.h \
	src/shapedmenu.h \
	src/point3d.h \
	src/view.h \
	src/highscoreview.h
SOURCES += src/stackedwidget.cpp \
	src/shapedmenu.cpp \
	src/main.cpp \
	src/highscoreview.cpp
FORMS += ui/stackedwidget.ui
