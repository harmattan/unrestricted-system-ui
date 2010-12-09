include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_dbusinterfacenotificationsource
INCLUDEPATH += $$NOTIFICATIONSRCDIR $$LIBNOTIFICATIONSRCDIR

# unit test and unit
SOURCES += \
    ut_dbusinterfacenotificationsource.cpp \
    $$NOTIFICATIONSRCDIR/dbusinterfacenotificationsource.cpp \
    $$NOTIFICATIONSRCDIR/mnotificationproxy.cpp \
    $$NOTIFICATIONSRCDIR/notificationsource.cpp \
    $$LIBNOTIFICATIONSRCDIR/notification.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationgroup.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationparameters.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationparameter.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp

# unit test and unit
HEADERS += \
    ut_dbusinterfacenotificationsource.h \
    $$NOTIFICATIONSRCDIR/dbusinterfacenotificationsource.h \
    $$NOTIFICATIONSRCDIR/mnotificationproxy.h \
    $$NOTIFICATIONSRCDIR/dbusinterfacenotificationsourceadaptor.h \
    $$NOTIFICATIONSRCDIR/notificationmanager.h \
    $$LIBNOTIFICATIONSRCDIR/notification.h \
    $$LIBNOTIFICATIONSRCDIR/notificationgroup.h \
    $$LIBNOTIFICATIONSRCDIR/notificationparameters.h \
    $$LIBNOTIFICATIONSRCDIR/notificationparameter.h

include(../common_bot.pri)
