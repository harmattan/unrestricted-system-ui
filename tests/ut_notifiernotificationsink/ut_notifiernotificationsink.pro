include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_notifiernotificationsink
INCLUDEPATH += $$SRCDIR/notifications

# unit test and unit
SOURCES += \
    ut_notifiernotificationsink.cpp \
    $$SRCDIR/notifications/notifiernotificationsink.cpp

# base classes
SOURCES += \
    $$SRCDIR/notifications/notificationsink.cpp \
    $$SRCDIR/notifications/notification.cpp \
    $$SRCDIR/notifications/notificationparameter.cpp \
    $$SRCDIR/notifications/notificationparameters.cpp

# service classes
SOURCES += $$STUBSDIR/stubbase.cpp

# unit test and unit
HEADERS += \
    ut_notifiernotificationsink.h \
    $$SRCDIR/notifications/notifiernotificationsink.h

# base classes
HEADERS += \
    $$SRCDIR/notifications/notificationsink.h \
    $$SRCDIR/notifications/notification.h \
    $$SRCDIR/notifications/notificationparameter.h \
    $$SRCDIR/notifications/notificationparameters.h \
    $$SRCDIR/sysuid.h \
    $$SRCDIR/notifications/notificationmanager.h \
    $$SRCDIR/notifications/notificationgroup.h \
    $$SRCDIR/notifications/eventtypestore.h

# service classes
HEADERS += $$STUBSDIR/stubbase.h

include(../common_bot.pri)
