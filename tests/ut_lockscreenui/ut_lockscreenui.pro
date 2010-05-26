include(../common_top.pri)
TARGET = ut_lockscreenui

SRC_PREFIX = ../../src
STUB_PREFIX = ../stubs

INCLUDEPATH += $$SRC_PREFIX \
               $$STUB_PREFIX

SOURCES += ut_lockscreenui.cpp \
    xchecker.cpp \
    $$SRC_PREFIX/debug.cpp \
    $$SRC_PREFIX/unlockwidgets.cpp \
    $$SRC_PREFIX/lockscreenui.cpp  \
    $$STUB_PREFIX/stubbase.cpp \
    $$SRC_PREFIX/unlocknotificationsink.cpp \
    $$SRC_PREFIX/notifications/notificationsink.cpp \
    $$SRC_PREFIX/notifications/notification.cpp \
    $$SRC_PREFIX/notifications/notificationgroup.cpp \
    $$SRC_PREFIX/notifications/notificationparameter.cpp \
    $$SRC_PREFIX/notifications/notificationparameters.cpp

HEADERS += ut_lockscreenui.h \
    xchecker.h \
    $$SRC_PREFIX/debug.h \
    $$SRC_PREFIX/unlockwidgets.h \
    $$SRC_PREFIX/lockscreenui.h \
    $$SRC_PREFIX/sysuid.h \
    $$SRC_PREFIX/unlocknotificationsink.h \
    $$SRC_PREFIX/notifications/notificationsink.h \
    $$SRC_PREFIX/notifications/notification.h \
    $$SRC_PREFIX/notifications/notificationgroup.h \
    $$SRC_PREFIX/notifications/notificationparameter.h \
    $$SRC_PREFIX/notifications/notificationparameters.h


include(../common_bot.pri)

