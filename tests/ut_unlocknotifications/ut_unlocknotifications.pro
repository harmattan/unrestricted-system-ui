include(../coverage.pri)
include(../common_top.pri)

TARGET = ut_unlocknotifications
INCLUDEPATH += $$SRCDIR/notifications $$SRCDIR $$STUBSDIR
DEFINES += NOTIFICATIONS_EVENT_TYPES=\'$$quote(\"$$M_NOTIFICATIONS_EVENT_TYPES_DIR\")\'

SOURCES += \
    ../common/MTester.cpp \
    ut_unlocknotifications.cpp \
    $$SRCDIR/unlocknotifications.cpp \
    $$SRCDIR/debug.cpp \
    $$STUBSDIR/stubbase.cpp \
    $$SRCDIR/notifications/notificationsink.cpp \
    $$SRCDIR/notifications/notification.cpp \
    $$SRCDIR/notifications/notificationgroup.cpp \
    $$SRCDIR/notifications/notificationparameter.cpp \
    $$SRCDIR/notifications/notificationparameters.cpp

HEADERS += \
    ../common/MTester.h \
    ut_unlocknotifications.h \
    $$SRCDIR/unlocknotifications.h \
    $$SRCDIR/debug.h \
    $$SRCDIR/unlockmissedevents.h \
    $$STUBSDIR/unlockmissedevents_stub.h \
    $$SRCDIR/notifications/notificationsink.h \
    $$SRCDIR/notifications/notification.h \
    $$SRCDIR/notifications/notificationgroup.h \
    $$SRCDIR/notifications/notificationparameter.h \
    $$SRCDIR/notifications/notificationparameters.h

include(../common_bot.pri)
