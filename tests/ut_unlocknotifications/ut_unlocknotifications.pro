include(../coverage.pri)
include(../common_top.pri)

TARGET = ut_unlocknotifications
INCLUDEPATH += $$NOTIFICATIONSRCDIR $$LIBNOTIFICATIONSRCDIR $$ROOTSRCDIR/extensions/screenlock
DEFINES += NOTIFICATIONS_EVENT_TYPES=\'$$quote(\"$$M_NOTIFICATIONS_EVENT_TYPES_DIR\")\'

SOURCES += \
    ../common/MTester.cpp \
    ut_unlocknotifications.cpp \
    $$ROOTSRCDIR/extensions/screenlock/unlocknotifications.cpp \
    $$STUBSDIR/stubbase.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationsink.cpp \
    $$LIBNOTIFICATIONSRCDIR/notification.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationgroup.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationparameter.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationparameters.cpp

HEADERS += \
    ../common/MTester.h \
    ut_unlocknotifications.h \
    $$ROOTSRCDIR/extensions/screenlock/unlocknotifications.h \
    $$ROOTSRCDIR/extensions/screenlock/unlockmissedevents.h \
    $$STUBSDIR/unlockmissedevents_stub.h \
    $$LIBNOTIFICATIONSRCDIR/notificationsink.h \
    $$LIBNOTIFICATIONSRCDIR/notification.h \
    $$LIBNOTIFICATIONSRCDIR/notificationgroup.h \
    $$LIBNOTIFICATIONSRCDIR/notificationparameter.h \
    $$LIBNOTIFICATIONSRCDIR/notificationparameters.h

include(../common_bot.pri)
