include(../coverage.pri)
include(../common_top.pri)

TARGET = ut_unlocknotifications
INCLUDEPATH += $$NOTIFICATIONSRCDIR $$LIBNOTIFICATIONSRCDIR $$SRCDIR/lockscreen
DEFINES += NOTIFICATIONS_EVENT_TYPES=\'$$quote(\"$$M_NOTIFICATIONS_EVENT_TYPES_DIR\")\'

SOURCES += \
    ../common/MTester.cpp \
    ut_unlocknotifications.cpp \
    $$SRCDIR/lockscreen/unlocknotifications.cpp \
    $$SRCDIR/debug.cpp \
    $$STUBSDIR/stubbase.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationsink.cpp \
    $$LIBNOTIFICATIONSRCDIR/notification.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationgroup.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationparameter.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationparameters.cpp

HEADERS += \
    ../common/MTester.h \
    ut_unlocknotifications.h \
    $$SRCDIR/lockscreen/unlocknotifications.h \
    $$SRCDIR/debug.h \
    $$SRCDIR/lockscreen/unlockmissedevents.h \
    $$STUBSDIR/unlockmissedevents_stub.h \
    $$LIBNOTIFICATIONSRCDIR/notificationsink.h \
    $$LIBNOTIFICATIONSRCDIR/notification.h \
    $$LIBNOTIFICATIONSRCDIR/notificationgroup.h \
    $$LIBNOTIFICATIONSRCDIR/notificationparameter.h \
    $$LIBNOTIFICATIONSRCDIR/notificationparameters.h

include(../common_bot.pri)
