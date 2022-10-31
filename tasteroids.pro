TEMPLATE = app
TARGET = tasteroids
CONFIG += c++11
android {
    QT += androidextras
    SOURCES += src/androidhelper.cpp #my android specific cpp file
    HEADERS += src/androidhelper.h   #my android specific header file
}

TRANSLATIONS=ta_ger.ts
QT += gui \
	core \
        svg multimedia

greaterThan(QT_MAJOR_VERSION, 5) {
    QT += svgwidgets
}
	
QT += qml quick widgets

QT += 3dcore 3drender 3dinput 3dquick qml quick 3dquickextras

HEADERS += src/vehicle.h \
	src/globalvariables.h \
	src/movingitem.h \
	src/centerrectinrect.h \
	src/graphicsview.h \
	src/graphicsengine.h \
	src/gamestate.h \
	src/svgcache.h \
	src/QGraphicsBox2DPolygonItem.h \
	src/contactlistener.h \
	src/signum.h \
	src/asteroidcolorizer.h \
	src/animateditem.h \
	src/isound.h \
	src/soundengine.h \
    src/Box2D/Collision/Shapes/b2CircleShape.h \
    src/Box2D/Collision/Shapes/b2PolygonShape.h \
    src/Box2D/Collision/Shapes/b2Shape.h \
    src/Box2D/Collision/b2BroadPhase.h \
    src/Box2D/Collision/b2Collision.h \
    src/Box2D/Collision/b2PairManager.h \
    src/Box2D/Common/b2BlockAllocator.h \
    src/Box2D/Common/b2Math.h \
    src/Box2D/Common/b2Settings.h \
    src/Box2D/Common/b2StackAllocator.h \
    src/Box2D/Common/Fixed.h \
    src/Box2D/Common/jtypes.h \
    src/Box2D/Dynamics/Contacts/b2CircleContact.h \
    src/Box2D/Dynamics/Contacts/b2Contact.h \
    src/Box2D/Dynamics/Contacts/b2ContactSolver.h \
    src/Box2D/Dynamics/Contacts/b2NullContact.h \
    src/Box2D/Dynamics/Contacts/b2PolyAndCircleContact.h \
    src/Box2D/Dynamics/Contacts/b2PolyContact.h \
    src/Box2D/Dynamics/Joints/b2DistanceJoint.h \
    src/Box2D/Dynamics/Joints/b2GearJoint.h \
    src/Box2D/Dynamics/Joints/b2Joint.h \
    src/Box2D/Dynamics/Joints/b2MouseJoint.h \
    src/Box2D/Dynamics/Joints/b2PrismaticJoint.h \
    src/Box2D/Dynamics/Joints/b2PulleyJoint.h \
    src/Box2D/Dynamics/Joints/b2RevoluteJoint.h \
    src/Box2D/Dynamics/b2Body.h \
    src/Box2D/Dynamics/b2ContactManager.h \
    src/Box2D/Dynamics/b2Island.h \
    src/Box2D/Dynamics/b2World.h \
    src/Box2D/Dynamics/b2WorldCallbacks.h \
    src/Box2D/Box2D.h \
    src/mocksound.h \
    src/sound.h \
    src/graphicssoftbutton.h \
    src/jsproxy.h \
    src/menugameinteraction.h \
    src/shapebuilder.h
SOURCES += src/vehicle.cpp \
	src/movingitem.cpp \
	src/graphicsview.cpp \
	src/graphicsengine.cpp \
	src/main.cpp \
	src/svgcache.cpp \
	src/asteroidcolorizer.cpp \
	src/animateditem.cpp \
	src/soundengine.cpp \
    src/Box2D/Collision/Shapes/b2CircleShape.cpp \
    src/Box2D/Collision/Shapes/b2PolygonShape.cpp \
    src/Box2D/Collision/Shapes/b2Shape.cpp \
    src/Box2D/Collision/b2BroadPhase.cpp \
    src/Box2D/Collision/b2CollideCircle.cpp \
    src/Box2D/Collision/b2CollidePoly.cpp \
    src/Box2D/Collision/b2Collision.cpp \
    src/Box2D/Collision/b2Distance.cpp \
    src/Box2D/Collision/b2PairManager.cpp \
    src/Box2D/Collision/b2TimeOfImpact.cpp \
    src/Box2D/Common/b2BlockAllocator.cpp \
    src/Box2D/Common/b2Math.cpp \
    src/Box2D/Common/b2Settings.cpp \
    src/Box2D/Common/b2StackAllocator.cpp \
    src/Box2D/Dynamics/Contacts/b2CircleContact.cpp \
    src/Box2D/Dynamics/Contacts/b2Contact.cpp \
    src/Box2D/Dynamics/Contacts/b2ContactSolver.cpp \
    src/Box2D/Dynamics/Contacts/b2PolyAndCircleContact.cpp \
    src/Box2D/Dynamics/Contacts/b2PolyContact.cpp \
    src/Box2D/Dynamics/Joints/b2DistanceJoint.cpp \
    src/Box2D/Dynamics/Joints/b2GearJoint.cpp \
    src/Box2D/Dynamics/Joints/b2Joint.cpp \
    src/Box2D/Dynamics/Joints/b2MouseJoint.cpp \
    src/Box2D/Dynamics/Joints/b2PrismaticJoint.cpp \
    src/Box2D/Dynamics/Joints/b2PulleyJoint.cpp \
    src/Box2D/Dynamics/Joints/b2RevoluteJoint.cpp \
    src/Box2D/Dynamics/b2Body.cpp \
    src/Box2D/Dynamics/b2ContactManager.cpp \
    src/Box2D/Dynamics/b2Island.cpp \
    src/Box2D/Dynamics/b2World.cpp \
    src/Box2D/Dynamics/b2WorldCallbacks.cpp \
    src/mocksound.cpp \
    src/sound.cpp \
    src/graphicssoftbutton.cpp \
    src/jsproxy.cpp \
    src/menugameinteraction.cpp \
    src/gamestate.cpp \
    src/shapebuilder.cpp
RESOURCES = \
	images.qrc \
	golevka.qrc \
	bacchus.qrc \
	explosion_images.qrc \
	kleopatra.qrc \
	ky26.qrc \
	toutatis.qrc \
	script.qrc \
        sounds.qrc \
        menu_files.qrc
	
SOURCES +=

HEADERS +=

RESOURCES += qml.qrc

RESOURCES +=  models.qrc


OTHER_FILES += src/mainloop.js \
	src/control.js 

DISTFILES += \
    android/AndroidManifest.xml \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/build.gradle \
    android/gradle.properties \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml \
    android/res/values/libs.xml \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
