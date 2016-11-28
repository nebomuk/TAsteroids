TEMPLATE = app

QT += qml quick widgets


# requires apt-get install qml-module-qtquick-localstorage

CONFIG += c++11

SOURCES += main.cpp \
    widgetinteraction.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

android {
    QT += androidextras
    SOURCES += androidhelper.cpp #my android specific cpp file
    HEADERS += androidhelper.h   #my android specific header file
}

HEADERS += \
    widgetinteraction.h
