QT -= core \
    gui
TARGET = Box2D
TEMPLATE = lib
CONFIG += staticlib
CONFIG -= debug_and_release debug_and_release_target
SOURCES += Collision/Shapes/b2Shape.cpp \
    Collision/Shapes/b2PolygonShape.cpp \
    Collision/Shapes/b2CircleShape.cpp \
    Collision/b2TimeOfImpact.cpp \
    Collision/b2PairManager.cpp \
    Collision/b2Distance.cpp \
    Collision/b2Collision.cpp \
    Collision/b2CollidePoly.cpp \
    Collision/b2CollideCircle.cpp \
    Collision/b2BroadPhase.cpp \
    Common/b2StackAllocator.cpp \
    Common/b2Settings.cpp \
    Common/b2Math.cpp \
    Common/b2BlockAllocator.cpp \
    Dynamics/Contacts/b2PolyContact.cpp \
    Dynamics/Contacts/b2PolyAndCircleContact.cpp \
    Dynamics/Contacts/b2ContactSolver.cpp \
    Dynamics/Contacts/b2Contact.cpp \
    Dynamics/Contacts/b2CircleContact.cpp \
    Dynamics/Joints/b2RevoluteJoint.cpp \
    Dynamics/Joints/b2PulleyJoint.cpp \
    Dynamics/Joints/b2PrismaticJoint.cpp \
    Dynamics/Joints/b2MouseJoint.cpp \
    Dynamics/Joints/b2Joint.cpp \
    Dynamics/Joints/b2GearJoint.cpp \
    Dynamics/Joints/b2DistanceJoint.cpp \
    Dynamics/b2WorldCallbacks.cpp \
    Dynamics/b2World.cpp \
    Dynamics/b2Island.cpp \
    Dynamics/b2ContactManager.cpp \
    Dynamics/b2Body.cpp
HEADERS += Box2D.h \
    Collision/Shapes/b2Shape.h \
    Collision/Shapes/b2PolygonShape.h \
    Collision/Shapes/b2CircleShape.h \
    Collision/b2PairManager.h \
    Collision/b2Collision.h \
    Collision/b2BroadPhase.h \
    Common/jtypes.h \
    Common/Fixed.h \
    Common/b2StackAllocator.h \
    Common/b2Settings.h \
    Common/b2Math.h \
    Common/b2BlockAllocator.h \
    Dynamics/Contacts/b2PolyContact.h \
    Dynamics/Contacts/b2PolyAndCircleContact.h \
    Dynamics/Contacts/b2NullContact.h \
    Dynamics/Contacts/b2ContactSolver.h \
    Dynamics/Contacts/b2Contact.h \
    Dynamics/Contacts/b2CircleContact.h \
    Dynamics/Joints/b2RevoluteJoint.h \
    Dynamics/Joints/b2PulleyJoint.h \
    Dynamics/Joints/b2PrismaticJoint.h \
    Dynamics/Joints/b2MouseJoint.h \
    Dynamics/Joints/b2Joint.h \
    Dynamics/Joints/b2GearJoint.h \
    Dynamics/Joints/b2DistanceJoint.h \
    Dynamics/b2WorldCallbacks.h \
    Dynamics/b2World.h \
    Dynamics/b2Island.h \
    Dynamics/b2ContactManager.h \
    Dynamics/b2Body.h
