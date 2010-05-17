include(../common_top.pri)

TARGET = ut_unlocknotificationsink
INCLUDEPATH += $$SRCDIR/notifications $$SRCDIR
DEFINES += NOTIFICATIONS_EVENT_TYPES=\'$$quote(\"$$M_NOTIFICATIONS_EVENT_TYPES_DIR\")\'

SOURCES += ut_unlocknotificationsink.cpp \
    $$SRCDIR/unlocknotificationsink.cpp \
    $$SRCDIR/debug.cpp \
    $$SRCDIR/notifications/notificationsink.cpp \
    $$SRCDIR/notifications/notification.cpp \
    $$SRCDIR/notifications/notificationgroup.cpp \
    $$SRCDIR/notifications/notificationparameter.cpp \
    $$SRCDIR/notifications/notificationparameters.cpp

HEADERS += ut_unlocknotificationsink.h \
    $$SRCDIR/unlocknotificationsink.h \
    $$SRCDIR/debug.h \
    $$SRCDIR/notifications/notificationsink.h \
    $$SRCDIR/notifications/notification.h \
    $$SRCDIR/notifications/notificationgroup.h \
    $$SRCDIR/notifications/notificationparameter.h \
    $$SRCDIR/notifications/notificationparameters.h

include(../common_bot.pri)
