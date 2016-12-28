TEMPLATE = app

QT += qml quick widgets

QT += 3dcore 3drender 3dinput 3dquick qml quick 3dquickextras



# requires apt-get install qml-module-qtquick-localstorage

CONFIG += c++11

SOURCES += src/menu/main.cpp \
    src/menu/widgetinteraction.cpp

RESOURCES += qml.qrc

RESOURCES +=  models.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

android {
    QT += androidextras
    SOURCES += src/androidhelper.cpp #my android specific cpp file
    HEADERS += src/androidhelper.h   #my android specific header file
}

HEADERS += \
    src/menu/widgetinteraction.h

FORMS +=
