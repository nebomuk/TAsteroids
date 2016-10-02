TARGET = tasteroids
CONFIG += warn_on
CONFIG += link_prl
DESTDIR = ../bin
MOC_DIR = build
RCC_DIR = build
OBJECTS_DIR = build
INCLUDEPATH += ../Box2D ../tasteroids6/src/ ../tasteroidsmenu3/src/
QMAKE_LIBDIR += ../Box2D ../tasteroids6 ../tasteroidsmenu3
win32:{
INCLUDEPATH += ../SDL
QMAKE_LIBDIR += ../SDL
}
LIBS += -lBox2D -Ltasteroids6 -ltasteroids -Ltasteroidsmenu3 -ltasteroidsmenu
UI_DIR = build
QT += opengl \
	svg \
	script

#HEADERS += shapedmenu.h

SOURCES += main.cpp
