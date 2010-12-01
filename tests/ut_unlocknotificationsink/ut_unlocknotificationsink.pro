include(../coverage.pri)
include(../common_top.pri)

TARGET = ut_unlocknotificationsink
INCLUDEPATH += $$NOTIFICATIONSRCDIR $$LIBNOTIFICATIONSRCDIR $$ROOTSRCDIR/extensions/screenlock
DEFINES += NOTIFICATIONS_EVENT_TYPES=\'$$quote(\"$$M_NOTIFICATIONS_EVENT_TYPES_DIR\")\'

SOURCES += ut_unlocknotificationsink.cpp \
    $$ROOTSRCDIR/extensions/screenlock/unlocknotificationsink.cpp \
    $$STUBSDIR/stubbase.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationsink.cpp \
    $$LIBNOTIFICATIONSRCDIR/notification.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationgroup.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationparameter.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationparameters.cpp

HEADERS += ut_unlocknotificationsink.h \
    $$ROOTSRCDIR/extensions/screenlock/unlocknotificationsink.h \
    $$ROOTSRCDIR/extensions/screenlock/unlockmissedevents.h \
    $$STUBSDIR/unlockmissedevents_stub.h \
    $$LIBNOTIFICATIONSRCDIR/notificationsink.h \
    $$LIBNOTIFICATIONSRCDIR/notification.h \
    $$LIBNOTIFICATIONSRCDIR/notificationgroup.h \
    $$LIBNOTIFICATIONSRCDIR/notificationparameter.h \
    $$LIBNOTIFICATIONSRCDIR/notificationparameters.h

include(../common_bot.pri)
