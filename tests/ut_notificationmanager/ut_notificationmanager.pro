include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_notificationmanager
INCLUDEPATH += $$SRCDIR/notifications
INCLUDEPATH += /usr/include/contextsubscriber
DEFINES += NOTIFICATIONS_EVENT_TYPES=\'$$quote(\"$$M_NOTIFICATIONS_EVENT_TYPES_DIR\")\'
LIBS += -L../../lib
# unit test and unit classes
SOURCES += \
    ut_notificationmanager.cpp \
    $$SRCDIR/notifications/notificationmanager.cpp \
    $$SRCDIR/contextframeworkcontext.cpp \
    $$SRCDIR/notifications/notificationsource.cpp \
    $$SRCDIR/notifications/notification.cpp \
    $$SRCDIR/notifications/notificationgroup.cpp \
    $$SRCDIR/notifications/notificationparameter.cpp \
    $$SRCDIR/notifications/notificationparameters.cpp

# unit test and unit classes
HEADERS += \
    ut_notificationmanager.h \
    $$SRCDIR/notifications/notificationmanager.h \
    $$SRCDIR/notifications/dbusinterfacenotificationsource.h \
    $$SRCDIR/applicationcontext.h \
    $$SRCDIR/contextframeworkcontext.h \
    $$SRCDIR/notifications/notificationsource.h \
    $$SRCDIR/notifications/notification.h \
    $$SRCDIR/notifications/notificationgroup.h \
    $$SRCDIR/notifications/notificationparameter.h \
    $$SRCDIR/notifications/notificationparameters.h \
    $$SRCDIR/notifications/eventtypestore.h

include(../common_bot.pri)
