include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_notificationmanager
INCLUDEPATH +=$$NOTIFICATIONSRCDIR $$LIBNOTIFICATIONSRCDIR
INCLUDEPATH += /usr/include/contextsubscriber
DEFINES += NOTIFICATIONS_EVENT_TYPES=\'$$quote(\"$$M_NOTIFICATIONS_EVENT_TYPES_DIR\")\'
LIBS += -L../../lib
# unit test and unit classes
SOURCES += \
    ut_notificationmanager.cpp \
    $$NOTIFICATIONSRCDIR/notificationmanager.cpp \
    $$NOTIFICATIONSRCDIR/mnotificationproxy.cpp \
    $$SRCDIR/contextframeworkcontext.cpp \
    $$NOTIFICATIONSRCDIR/notificationsource.cpp \
    $$LIBNOTIFICATIONSRCDIR/notification.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationgroup.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationparameter.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationparameters.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp

# unit test and unit classes
HEADERS += \
    ut_notificationmanager.h \
    $$NOTIFICATIONSRCDIR/notificationmanager.h \
    $$NOTIFICATIONSRCDIR/dbusinterfacenotificationsource.h \
    $$NOTIFICATIONSRCDIR/dbusinterfacenotificationsink.h \
    $$NOTIFICATIONSRCDIR/mnotificationproxy.h \
    $$SRCDIR/applicationcontext.h \
    $$SRCDIR/contextframeworkcontext.h \
    $$NOTIFICATIONSRCDIR/notificationsource.h \
    $$LIBNOTIFICATIONSRCDIR/notification.h \
    $$LIBNOTIFICATIONSRCDIR/notificationgroup.h \
    $$LIBNOTIFICATIONSRCDIR/notificationparameter.h \
    $$LIBNOTIFICATIONSRCDIR/notificationparameters.h \
    $$LIBNOTIFICATIONSRCDIR/notificationsink.h \
    $$NOTIFICATIONSRCDIR/eventtypestore.h

include(../common_bot.pri)
